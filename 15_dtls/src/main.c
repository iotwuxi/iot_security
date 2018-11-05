#include <zephyr.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

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

#define SERVER_PORT             "4432"
#define SERVER_ADDR             "iotwuxi.org"
#define MESSAGE 				"Echo this\r\n"

// 000102030405060708090a0b0c0d0e0f
const uint8_t psk[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
const char psk_id[] = "Client_identity";

struct dtls_timing_context 
{
    uint32_t snapshot;
    uint32_t int_ms;
    uint32_t fin_ms;
};
static struct dtls_timing_context timer;

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

    if (elapsed_ms >= ctx->fin_ms)
        return 2;

    if (elapsed_ms >= ctx->int_ms)
        return 1;
    return 0;
}

static int entropy_source(void *data, uint8_t *output, size_t len, size_t *olen)
{
    uint32_t seed;
    ARG_UNUSED(data);

    seed = sys_rand32_get();
    if (len > sizeof(seed)) {
        len = sizeof(seed);
    }

    memcpy(output, &seed, len);
    *olen = len;

    return 0;
}

int main(void)
{ 
    int ret = 0, len = 0;
    unsigned char buf[256];
    const char *pers = "dtls_client";
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_platform_set_printf(printf);

    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_net_context ctx; 
    
    mbedtls_net_init(&ctx);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_printf("\n  . Seeding the random number generator...");
    mbedtls_entropy_init(&entropy);
    mbedtls_entropy_add_source(&entropy, entropy_source, NULL,
                   MBEDTLS_ENTROPY_MAX_GATHER, MBEDTLS_ENTROPY_SOURCE_STRONG);
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                      (const uint8_t *)pers, strlen(pers));
    assert_exit(ret == 0, ret);
    
    mbedtls_printf(" ok\n  . Setting up the SSL/TLS structure...");
    ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT,
                MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_PRESET_DEFAULT);
    assert_exit(ret == 0, ret);

    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    mbedtls_ssl_conf_psk(&conf, psk, sizeof(psk),
                 (const uint8_t *)psk_id, strlen((char*)psk_id));

    ret = mbedtls_ssl_setup(&ssl, &conf);
    assert_exit(ret == 0, ret);
    
    mbedtls_printf(" ok\n  . Connecting to %s:%s...", SERVER_ADDR, SERVER_PORT);
    ret = mbedtls_net_connect( &ctx, SERVER_ADDR, SERVER_PORT, MBEDTLS_NET_PROTO_UDP );
    assert_exit(ret == 0, ret);
    mbedtls_ssl_set_timer_cb(&ssl, &timer, dtls_timing_set_delay, dtls_timing_get_delay);
    mbedtls_ssl_set_bio( &ssl, &ctx, mbedtls_net_send, mbedtls_net_recv, NULL );
    
    mbedtls_printf(" ok\n  . Performing the SSL/TLS handshake...");
    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            mbedtls_printf(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret);
            goto cleanup;
        }
    }

    mbedtls_printf(" ok\n  > Write to server:");
    do {
        ret = mbedtls_ssl_write(&ssl, (const uint8_t *)MESSAGE, strlen(MESSAGE));
    } while (ret == MBEDTLS_ERR_SSL_WANT_READ ||
         ret == MBEDTLS_ERR_SSL_WANT_WRITE);
    assert_exit(ret > 0, ret);
    len = ret;
    mbedtls_printf( " %d bytes written\n\n%s\n", len, MESSAGE);

    mbedtls_printf("  > Read from server:");
    len = sizeof(buf) - 1;
    memset(buf, 0x00, sizeof(buf));

    do {
        ret = mbedtls_ssl_read(&ssl, buf, len);
    } while (ret == MBEDTLS_ERR_SSL_WANT_READ ||
         ret == MBEDTLS_ERR_SSL_WANT_WRITE);
    assert_exit(ret > 0, ret);

    len = ret;
    mbedtls_printf( " %d bytes read\n\n%s\n", len, buf);
    
    mbedtls_ssl_close_notify(&ssl);
    mbedtls_printf(" . Closing the connection ... done\n");

cleanup:
    mbedtls_net_free(&ctx);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return ret;
}
