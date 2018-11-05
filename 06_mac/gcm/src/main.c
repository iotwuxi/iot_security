/*
 * @file
 * @brief chapter gcm samples.
 * @note  测试向量来自 http://csrc.nist.gov/groups/STM/cavp/documents/mac/gcmtestvectors.zip
*/
#include <zephyr.h>

#include <stdio.h>
#include <string.h>

#include "mbedtls/cipher.h"
#include "mbedtls/platform.h"

/*
    [Keylen = 128]
    [IVlen = 96]
    [PTlen = 128]
    [AADlen = 128]
    [Taglen = 128]

    Count = 0
    Key = c939cc13397c1d37de6ae0e1cb7c423c
    IV = b3d8cc017cbb89b39e0f67e2
    PT = c3b3c41f113a31b73d9a5cd432103069
    AAD = 24825602bd12a984e0092d3e448eda5f
    CT = 93fe7d9e9bfd10348a5606e5cafa7354
    Tag = 0032a1dc85f1c9786925a2e71d8272dd
*/
static uint8_t key[] =
{
    0xc9, 0x39, 0xcc, 0x13, 0x39, 0x7c, 0x1d, 0x37, 
    0xde, 0x6a, 0xe0, 0xe1, 0xcb, 0x7c, 0x42, 0x3c
};

static uint8_t iv[] =
{
    0xb3, 0xd8, 0xcc, 0x01, 0x7c, 0xbb, 0x89, 0xb3, 
    0x9e, 0x0f, 0x67, 0xe2,
};

static uint8_t pt[] =
{
    0xc3, 0xb3, 0xc4, 0x1f, 0x11, 0x3a, 0x31, 0xb7, 
    0x3d, 0x9a, 0x5c, 0xd4, 0x32, 0x10, 0x30, 0x69
};

static uint8_t add[] = 
{
    0x24, 0x82, 0x56, 0x02, 0xbd, 0x12, 0xa9, 0x84, 
    0xe0, 0x09, 0x2d, 0x3e, 0x44, 0x8e, 0xda, 0x5f
};

static uint8_t ct[] =
{
    0x93, 0xfe, 0x7d, 0x9e, 0x9b, 0xfd, 0x10, 0x34, 
    0x8a, 0x56, 0x06, 0xe5, 0xca, 0xfa, 0x73, 0x54
};

static uint8_t tag[] =
{
    0x00, 0x32, 0xa1, 0xdc, 0x85, 0xf1, 0xc9, 0x78, 
    0x69, 0x25, 0xa2, 0xe7, 0x1d, 0x82, 0x72, 0xdd
};

#define assert_exit(cond, ret) \
    do { if (!(cond)) { \
        printf("  !. assert: failed [line: %d, error: -0x%04X]\n", __LINE__, -ret); \
        goto cleanup; \
    } } while (0)

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
    int ret;
    size_t len;
    uint8_t buf[16], tag_buf[16];

    mbedtls_cipher_context_t ctx;
    const mbedtls_cipher_info_t *info;

    mbedtls_platform_set_printf(printf);

    mbedtls_cipher_init(&ctx);
    info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_GCM);

    mbedtls_cipher_setup(&ctx, info);
    mbedtls_printf("\n  cipher info setup, name: %s, block size: %d\n", 
                        mbedtls_cipher_get_name(&ctx), 
                        mbedtls_cipher_get_block_size(&ctx));

    mbedtls_cipher_setkey(&ctx, key, sizeof(key)*8, MBEDTLS_ENCRYPT);

    ret = mbedtls_cipher_auth_encrypt(&ctx, iv, sizeof(iv), add, sizeof(add),
                                        pt, sizeof(pt), buf, &len, tag_buf, 16);
    assert_exit(ret == 0, ret);
    assert_exit(memcmp(buf, ct, sizeof(ct)) == 0, -1);
    assert_exit(memcmp(tag_buf, tag, 16) == 0, -1);
    dump_buf("\n  cipher gcm auth encrypt:", buf, 16);
    dump_buf("\n  cipher gcm auth tag:", tag_buf, 16);

    mbedtls_cipher_setkey(&ctx, key, sizeof(key)*8, MBEDTLS_DECRYPT);
    ret = mbedtls_cipher_auth_decrypt(&ctx, iv, sizeof(iv), add, sizeof(add),
                                        ct, sizeof(ct), buf, &len, tag, 16);
    assert_exit(ret == 0, ret);
    assert_exit(memcmp(buf, pt, sizeof(pt)) == 0, -1);
    dump_buf("\n  cipher gcm auth decrypt:", buf, 16);

cleanup:
    mbedtls_cipher_free(&ctx);

    return(0);
}
