#include <zephyr.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <net/coap.h>
#include <net/buf.h>
#include <net/net_pkt.h>

#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/platform.h"

#define assert_exit(cond, ret) \
    do { if (!(cond)) { \
        printf("  !. assert: failed [line: %d, error: -0x%04X]\n", __LINE__, -ret); \
        goto cleanup; \
    } } while (0)

#define SERVER_PORT   "5682"
#define SERVER_ADDR   "iotwuxi.org"

const char psk_id[] = "identity\0";
const uint8_t psk[] = "password\0";
const char *const uri_path = "time";

#define COAP_BUF_SIZE 128
NET_PKT_TX_SLAB_DEFINE(coap_pkt_slab, 1);
NET_BUF_POOL_DEFINE(coap_data_pool, 1, COAP_BUF_SIZE, 0, NULL);

struct dtls_timing_context 
{
    u32_t snapshot;
    u32_t int_ms;
    u32_t fin_ms;
};

void dtls_timing_set_delay(void *data, uint32_t int_ms, uint32_t fin_ms)
{
    struct dtls_timing_context *ctx = (struct dtls_timing_context *)data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if (fin_ms != 0) {
        ctx->snapshot = k_uptime_get_32();
    }
}

int dtls_timing_get_delay(void *data)
{
    struct dtls_timing_context *ctx = (struct dtls_timing_context *)data;
    unsigned long elapsed_ms;

    if (ctx->fin_ms == 0) {
        return -1;
    }

    elapsed_ms = k_uptime_get_32() - ctx->snapshot;

    if (elapsed_ms >= ctx->fin_ms) {
        return 2;
    }

    if (elapsed_ms >= ctx->int_ms) {
        return 1;
    }

    return 0;
}

static int entropy_source(void *data, uint8_t *output, size_t len,
              size_t *olen)
{
    u32_t seed;

    ARG_UNUSED(data);

    seed = sys_rand32_get();

    if (len > sizeof(seed)) {
        len = sizeof(seed);
    }

    memcpy(output, &seed, len);

    *olen = len;

    return 0;
}

void main(void)
{
    int ret;
    uint16_t len, offset;
    struct net_pkt *pkt;
    struct net_buf *frag;
    struct coap_packet request, cpkt;
    struct dtls_timing_context timer;
    const char *pers = "coaps_client";

    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_net_context ctx; 
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_platform_set_printf(printf);

    mbedtls_net_init(&ctx);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_printf("\n  . Seeding the random number generator ...");
    mbedtls_entropy_init(&entropy);
    mbedtls_entropy_add_source(&entropy, entropy_source, NULL,
                   MBEDTLS_ENTROPY_MAX_GATHER,
                   MBEDTLS_ENTROPY_SOURCE_STRONG);

    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                    (const uint8_t *)pers, strlen(pers));
    assert_exit(ret == 0, ret);

    mbedtls_printf(" ok\n  . Setting up the SSL/TLS structure ...");
    ret = mbedtls_ssl_config_defaults(&conf,
                      MBEDTLS_SSL_IS_CLIENT,
                      MBEDTLS_SSL_TRANSPORT_DATAGRAM,
                      MBEDTLS_SSL_PRESET_DEFAULT);
    assert_exit(ret == 0, ret);

    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    ret = mbedtls_ssl_setup(&ssl, &conf);
    assert_exit(ret == 0, ret);

    ret = mbedtls_net_connect(&ctx, SERVER_ADDR, SERVER_PORT, MBEDTLS_NET_PROTO_UDP);
    mbedtls_printf(" ok\n  . Connecting to %s:%s ...", SERVER_ADDR, SERVER_PORT);
    
    ret = mbedtls_ssl_conf_psk(&conf, psk, strlen((char *)psk),
                                (uint8_t *)psk_id, strlen(psk_id));
    assert_exit(ret == 0, ret);

    mbedtls_ssl_set_timer_cb(&ssl, &timer, dtls_timing_set_delay,
                 dtls_timing_get_delay);

    mbedtls_ssl_set_bio(&ssl, &ctx, mbedtls_net_send, mbedtls_net_recv, NULL);

    mbedtls_printf(" ok\n  . Performing the SSL/TLS handshake ...");
    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            mbedtls_printf(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret);
            goto cleanup;
        }
    }
    
    mbedtls_printf(" ok\n  > Set coap packet ...");
    pkt = net_pkt_get_reserve(&coap_pkt_slab, 0, K_NO_WAIT);
    assert_exit(pkt != 0, -1);
    frag = net_buf_alloc(&coap_data_pool, K_NO_WAIT);
    assert_exit(frag != 0, -1);
    net_pkt_frag_add(pkt, frag);
    ret = coap_packet_init(&request, pkt, 1, COAP_TYPE_CON, 8, 
                        coap_next_token(), COAP_METHOD_GET, coap_next_id());
    assert_exit(ret >= 0, -1);
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));
    assert_exit(ret >= 0, -1);

    mbedtls_printf(" ok\n  > Write to server ...");
    ret = mbedtls_ssl_write(&ssl, frag->data, frag->len);
    assert_exit(ret > 0, ret);

    mbedtls_printf(" ok\n  > Read from server ...");
    uint8_t *response = frag->data + NET_UDPH_LEN;
    ret = mbedtls_ssl_read(&ssl, response, COAP_BUF_SIZE - 1);
    assert_exit(ret > 0, ret);
    frag->len = ret + NET_UDPH_LEN;
    ret = coap_packet_parse(&cpkt, pkt, NULL, 0);
    assert_exit(ret >= 0, -1);

    mbedtls_printf(" ok\n  > Parse coap packet ...");
    len = COAP_BUF_SIZE - 1;
    frag = coap_packet_get_payload(&cpkt, &offset, &len);
    mbedtls_printf(" ok\n\n  . [COAP] Response Code: %d\n", coap_header_get_code(&cpkt));
    mbedtls_printf("  . [COAP] Response Message id: %d\n", coap_header_get_id(&cpkt));
    mbedtls_printf("  . [COAP] Response Payload: %s\n", frag->data + offset);

    net_pkt_unref(pkt);
    mbedtls_ssl_close_notify(&ssl);

cleanup:
    mbedtls_net_free(&ctx);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
}
