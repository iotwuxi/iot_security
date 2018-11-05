#include <zephyr.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/platform.h"

#include "ca_cert.h"

#if defined(MBEDTLS_DEBUG_C)
#include "mbedtls/debug.h"
#define DEBUG_THRESHOLD 4

static void my_debug(void *ctx, int level,
                     const char *file, int line, const char *str)
{
    const char *p, *basename;

    /* Extract basename from file */
    for (p = basename = file; *p != '\0'; p++)
    {
        if (*p == '/' || *p == '\\')
        {
            basename = p + 1;
        }
    }

    mbedtls_printf("%s:%04d: |%d| %s", basename, line, level, str);
}
#endif

#define assert_exit(cond, ret) \
    do { if (!(cond)) { \
        printf("  !. assert: failed [line: %d, error: -0x%04X]\n", __LINE__, -(ret)); \
        goto cleanup; \
    } } while (0)

#define SERVER_ADDR         "iotwuxi.org"
#define SERVER_PORT         "442"
#define HOSTNAME            "iotwuxi.org"
#define GET_REQUEST         "GET /index.html HTTP/1.0\r\n\r\n"

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
    uint8_t buf[256];
    int ret = 0, len = 0;
    const char *pers = "tls_client";
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_platform_set_printf(printf);
    mbedtls_platform_set_snprintf(snprintf);

    mbedtls_x509_crt ca;
    mbedtls_net_context ctx;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    
    mbedtls_net_init(&ctx);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_x509_crt_init(&ca);

    mbedtls_printf("\n  tls client use %s board.\n", CONFIG_BOARD);
    mbedtls_printf("\n  . Seeding the random number generator ... ");
    mbedtls_entropy_init(&entropy);
    mbedtls_entropy_add_source(&entropy, entropy_source, NULL,
               MBEDTLS_ENTROPY_MAX_GATHER, MBEDTLS_ENTROPY_SOURCE_STRONG);

    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                  (const uint8_t *)pers, strlen(pers));
    assert_exit(ret == 0, ret);

    mbedtls_printf(" ok\n  . Setting up the SSL/TLS structure ... ");
    ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT,
                        MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    assert_exit(ret == 0, ret);

    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    ret = mbedtls_x509_crt_parse_der(&ca, ca_cert_der, ca_cert_der_len);
    assert_exit(ret == 0, ret);

    mbedtls_ssl_conf_ca_chain(&conf, &ca, NULL);
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(DEBUG_THRESHOLD);
    mbedtls_ssl_conf_dbg(&conf, my_debug, NULL);
#endif

    ret = mbedtls_ssl_setup(&ssl, &conf);
    assert_exit(ret == 0, ret);
    
    ret = mbedtls_ssl_set_hostname(&ssl, HOSTNAME);
    assert_exit(ret == 0, ret);

    mbedtls_printf(" ok\n  . Connecting to %s:%s...", SERVER_ADDR, SERVER_PORT);
    ret = mbedtls_net_connect( &ctx, SERVER_ADDR, SERVER_PORT, MBEDTLS_NET_PROTO_TCP );
    assert_exit(ret == 0, ret);
    mbedtls_ssl_set_bio( &ssl, &ctx, mbedtls_net_send, mbedtls_net_recv, NULL );

    mbedtls_printf(" ok\n  . Performing the SSL/TLS handshake ...");
    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            mbedtls_printf(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret);
            goto cleanup;
        }
    }

    mbedtls_printf(" ok\n  > Write to server:");
    ret = mbedtls_ssl_write(&ssl, (const uint8_t *)GET_REQUEST, strlen(GET_REQUEST));
    assert_exit(ret > 0, ret);

    len = ret;
    mbedtls_printf( " %d bytes written\n\n%s\n\n", len, GET_REQUEST);

    mbedtls_printf("  > Read from server:");
    len = sizeof(buf) - 1;
    memset(buf, 0x00, sizeof(buf));

    do {
        ret = mbedtls_ssl_read(&ssl, buf, len);
    } while (ret == MBEDTLS_ERR_SSL_WANT_READ ||
         ret == MBEDTLS_ERR_SSL_WANT_WRITE);
    assert_exit(ret > 0, ret);

    len = ret;
    mbedtls_printf( " %d bytes read\n\n\n%s\n\n", len, buf);
    
    mbedtls_ssl_close_notify(&ssl);
    mbedtls_printf(" ok\n  . Closing the connection ... done\n");

cleanup:
    mbedtls_net_free(&ctx);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    mbedtls_x509_crt_free(&ca);

    return ret;
}
