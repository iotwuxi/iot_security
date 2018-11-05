/*
 * @file
 * @brief chapter hash samples.
 * @note  测试向量来自 https://tools.ietf.org/rfc/rfc4231.txt (test case 2)
*/
#include <zephyr.h>

#include <stdio.h>
#include <string.h>

#include "mbedtls/md.h"
#include "mbedtls/platform.h"

static void dump_buf(char *info, uint8_t *buf, uint32_t len)
{
    mbedtls_printf("%s", info);
    for (int i = 0; i < len; i++) {
        mbedtls_printf("%s%02X%s", i % 16 == 0 ? "\n     ":" ", 
                        buf[i], i == len - 1 ? "\n":"");
    }
}

int main(void)
{
    uint8_t mac[32];
    char *secret = "Jefe";
    char *msg = "what do ya want for nothing?";

    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *info;

    mbedtls_platform_set_printf(printf);

    mbedtls_md_init(&ctx);
    info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    mbedtls_md_setup(&ctx, info, 1);
    mbedtls_printf("\n  md info setup, name: %s, digest size: %d\n", 
                   mbedtls_md_get_name(info), mbedtls_md_get_size(info));

    mbedtls_md_hmac_starts(&ctx, secret, strlen(secret));
    mbedtls_md_hmac_update(&ctx, msg, strlen(msg));
    mbedtls_md_hmac_finish(&ctx, mac);

    dump_buf("\n  md hmac-sha-256 mac:", mac, sizeof(mac));

    mbedtls_md_free(&ctx);  

    return 0;
}
