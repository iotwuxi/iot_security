#include <zephyr.h>

#include <stdio.h>
#include <string.h>

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/bignum.h"
#include "mbedtls/platform.h"

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

static int entropy_source(void *data, uint8_t *output, size_t len, size_t *olen)
{
    uint32_t seed;

    seed = sys_rand32_get();
    if(len > sizeof(seed)) {
        len = sizeof(seed);
    }
    memcpy(output, &seed, len);

    *olen = len;
    return 0;
}

int main(void)
{
    int ret = 0;
    uint8_t prime[64];
    mbedtls_mpi P, Q;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const uint8_t *pers = "CTR_DRBG";

    mbedtls_platform_set_printf(printf);
  
    mbedtls_mpi_init(&P); 
    mbedtls_mpi_init(&Q);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_entropy_add_source(&entropy, entropy_source, NULL,
                               MBEDTLS_ENTROPY_MAX_GATHER,
                               MBEDTLS_ENTROPY_SOURCE_STRONG);
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, 
                                (const unsigned char *) pers, strlen(pers));
    assert_exit(ret == 0, ret);
    mbedtls_printf("\n  . setup rng ... ok\n");

    mbedtls_printf("\n  ! Generating large primes may take minutes!\n");  
    ret = mbedtls_mpi_gen_prime(&P, sizeof(prime)*8, 1, 
                                mbedtls_ctr_drbg_random, &ctr_drbg);
    assert_exit(ret == 0, ret);

    ret = mbedtls_mpi_sub_int(&Q, &P, 1);
    assert_exit(ret == 0, ret);

    ret = mbedtls_mpi_div_int(&Q, NULL, &Q, 2);
    assert_exit(ret == 0, ret);

    ret = mbedtls_mpi_is_prime(&Q, mbedtls_ctr_drbg_random, &ctr_drbg);
    assert_exit(ret == 0, ret);
    mbedtls_printf("\n  . Verifying that Q = (P-1)/2 is prime ... ok\n");

    mbedtls_mpi_write_binary(&P, prime, sizeof(prime));
    dump_buf("\n  . generate 512 bit prime data ... ok", prime, sizeof(prime));

cleanup:
    mbedtls_mpi_free(&P); 
    mbedtls_mpi_free(&Q);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return 0;
}
