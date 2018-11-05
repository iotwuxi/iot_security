/*
 * @file
 * @brief chapter start samples.
 * @note  测试向量来自 https://tools.ietf.org/html/rfc3548
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "mbedtls/base64.h"
#include "mbedtls/platform.h"

static uint8_t msg[] = 
{
    0x14, 0xfb, 0x9c, 0x03, 0xd9, 0x7e
};

void dump_buf(char *info, uint8_t *buf, uint32_t len)
{
    mbedtls_printf("%s", info);
    for(uint32_t i = 0; i < len; i++) {
        mbedtls_printf("%02x ", msg[i]);
    }
    mbedtls_printf("\n");
}

int main(void)
{
    size_t len;
    uint8_t rst[512];

    len = sizeof(msg);
    dump_buf("\n  base64 message: ", msg, len);

    mbedtls_base64_encode(rst, sizeof(rst), &len, msg, len);
    mbedtls_printf("  base64 encode : %s\n", rst);

    mbedtls_base64_decode(rst, sizeof(rst), &len, rst, len);
    dump_buf("  base64 decode : ", rst, len);
    printf("\n");

    return 0;
}