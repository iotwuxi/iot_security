#include <zephyr.h>

#include <stdio.h>
#include <string.h>

#include "mbedtls/x509_crt.h"
#include "mbedtls/platform.h"

#include "certs.h"

#define assert_exit(cond, ret) \
    do { if (!(cond)) { \
        printf("  !. assert: failed [line: %d, error: -0x%04X]\n", __LINE__, -ret); \
        goto cleanup; \
    } } while (0)

static int my_verify(void *data, mbedtls_x509_crt *crt, 
                                int depth, uint32_t *flags)
{
    ((void) data);
    char buf[1024];
    int ret = mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "      ", crt);
    mbedtls_printf("  . Verify requested for (Depth %d) ... ok\n", depth);
    for(uint32_t i = 0; i < ret; i++) {
        mbedtls_printf("%c", buf[i]);
    }

    if ((*flags) != 0) {
        ret = mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", *flags);
        for(uint32_t i = 0; i < ret; i++) {
            mbedtls_printf("%c", buf[i]);
        }
    }
    mbedtls_printf("\n");

    return(0);
}

int main(void)
{
    int ret = 0; 
    uint32_t flags = 0;
    mbedtls_x509_crt cert, cacert;

    mbedtls_platform_set_printf(printf);
    mbedtls_platform_set_snprintf(snprintf);

    mbedtls_x509_crt_init(&cert); 
    mbedtls_x509_crt_init(&cacert);

    ret = mbedtls_x509_crt_parse(&cert, bob_cert, sizeof(bob_cert));
    assert_exit(ret == 0, ret);
    ret = mbedtls_x509_crt_parse(&cacert, ca_cert, sizeof(ca_cert));
    assert_exit(ret == 0, ret);
    mbedtls_printf("\n  . Loading the certificate(s) ... ok\n\n");

    ret = mbedtls_x509_crt_verify(&cert, &cacert, NULL, NULL, &flags, my_verify, NULL);
    assert_exit(ret == 0, ret);

cleanup:
    mbedtls_x509_crt_free(&cert); 
    mbedtls_x509_crt_free(&cacert);

    return(ret != 0);
}
