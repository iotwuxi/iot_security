#include <zephyr.h>

#include <string.h>
#include <stdio.h>

#include "mbedtls/cipher.h"
#include "mbedtls/platform.h"

/*
    # padding with pkcs7 AES_128_CBC Encrypt
    ptx = "CBC has been the most commonly used mode of operation."
    key = 06a9214036b8a15b512e03d534120006
    iv  = 3dafba429d9eb430b422da802c9fac41
    ctx = 4DDF9012D7B3898745A1ED9860EB0FA2
          FD2BBD80D27190D72A2F240C8F372A27
          63746296DDC2BFCE7C252B6CD7DD4BA8
          577E096DBD8024C8B4C5A1160CA2D3F9
*/
char *ptx = "CBC has been the most commonly used mode of operation.";
uint8_t key[16] =
{
    0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b,
    0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06
};

uint8_t iv[16] =
{
    0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30,
    0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41
};

static void dump_buf(char *info, uint8_t *buf, uint32_t len)
{
    mbedtls_printf("%s", info);
    for (int i = 0; i < len; i++) {
        mbedtls_printf("%s%02X%s", i % 16 == 0 ? "\n\t":" ", 
                        buf[i], i == len - 1 ? "\n":"");
    }
    mbedtls_printf("\n");
}

void cipher(int type)
{
    size_t len;
    int olen = 0;
    uint8_t buf[64];

    mbedtls_cipher_context_t ctx;
    const mbedtls_cipher_info_t *info;

    mbedtls_platform_set_printf(printf);

    mbedtls_cipher_init(&ctx);
    info = mbedtls_cipher_info_from_type(type);

    mbedtls_cipher_setup(&ctx, info);
    mbedtls_printf("\n  cipher info setup, name: %s, block size: %d\n", 
                        mbedtls_cipher_get_name(&ctx), 
                        mbedtls_cipher_get_block_size(&ctx));

    mbedtls_cipher_setkey(&ctx, key, sizeof(key)*8, MBEDTLS_ENCRYPT);
    mbedtls_cipher_set_iv(&ctx, iv, sizeof(iv));
    mbedtls_cipher_update(&ctx, ptx, strlen(ptx), buf, &len);
    olen += len;

    mbedtls_cipher_finish(&ctx, buf + len, &len);
    olen += len;

    dump_buf("\n  cipher aes encrypt:", buf, olen);

    mbedtls_cipher_free(&ctx);
}

int main(void)
{
    cipher(MBEDTLS_CIPHER_AES_128_CBC);
    cipher(MBEDTLS_CIPHER_AES_128_CTR);

    return 0;
}
