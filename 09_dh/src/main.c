#include <zephyr.h>

#include <stdio.h>
#include <string.h>

#include "mbedtls/dhm.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/platform.h"

#define assert_exit(cond, ret) \
    do { if (!(cond)) { \
        printf("  !. assert: failed [line: %d, error: -0x%04X]\n", __LINE__, -ret); \
        goto cleanup; \
    } } while (0)

#define GENERATOR   "2"
#define T_P          "FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695" \
                     "A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617A"\
                     "D3DF1ED5D5FD65612433F51F5F066ED0856365553DED1AF3B557135E7F57C935"\
                     "984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797A"\
                     "BC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4"\
                     "AE56EDE76372BB190B07A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F61"\
                     "9172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005"\
                     "C58EF1837D1683B2C6F34A26C1B2EFFA886B423861285C97FFFFFFFFFFFFFFFF"

static int entropy_source(void *data, uint8_t *output, size_t len, size_t *olen)
{
    uint32_t seed;

    seed = sys_rand32_get();
    if (len > sizeof(seed)) {
        len = sizeof(seed);
    }

    memcpy(output, &seed, len);

    *olen = len;
    return 0;
}

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
    int ret = 0;
    size_t n = 0;
    const char *pers = "simple_dh";
    uint8_t cli_pub[256], cli_secret[256];
    uint8_t srv_pub[256], srv_secret[256];

    mbedtls_platform_set_printf(printf);

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_dhm_context dhm_cli, dhm_srv;

    mbedtls_dhm_init(&dhm_cli); 
    mbedtls_dhm_init(&dhm_srv);
    mbedtls_entropy_init(&entropy); 
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_entropy_add_source(&entropy, entropy_source, NULL,
                    MBEDTLS_ENTROPY_MAX_GATHER, MBEDTLS_ENTROPY_SOURCE_STRONG);
    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, 
                                (const unsigned char *) pers, strlen(pers));
    assert_exit(ret == 0, ret);
    mbedtls_printf("\n  . setup rng ... ok\n\n");

    mbedtls_mpi_read_string(&dhm_srv.P, 16, T_P);
    mbedtls_mpi_read_string(&dhm_srv.G, 10, GENERATOR);
    dhm_srv.len = mbedtls_mpi_size(&dhm_srv.P);

    mbedtls_mpi_read_string(&dhm_cli.P, 16, T_P);
    mbedtls_mpi_read_string(&dhm_cli.G, 10, GENERATOR);
    dhm_cli.len = mbedtls_mpi_size(&dhm_cli.P);
    mbedtls_printf("  1. dh generate 2048 bit prime(G, P) ... ok\n");

    ret = mbedtls_dhm_make_public(&dhm_srv, 256, srv_pub, sizeof(srv_pub), 
                                            mbedtls_ctr_drbg_random, &ctr_drbg);
    assert_exit(ret == 0, ret);
    dump_buf("  2. dh server generate public parameter:", srv_pub, sizeof(srv_pub));

    ret = mbedtls_dhm_make_public(&dhm_cli, 256, cli_pub, sizeof(cli_pub), 
                                            mbedtls_ctr_drbg_random, &ctr_drbg);
    assert_exit(ret == 0, ret);
    dump_buf("  3. dh client generate public parameter:", cli_pub, sizeof(cli_pub));

    ret = mbedtls_dhm_read_public(&dhm_srv, cli_pub, sizeof(cli_pub));
    assert_exit(ret == 0, ret);
    mbedtls_printf("  4. dh server read public ... ok\n"); 

    ret = mbedtls_dhm_read_public(&dhm_cli, srv_pub, sizeof(srv_pub));
    assert_exit(ret == 0, ret);
    mbedtls_printf("  5. dh client read public ... ok\n");

    ret = mbedtls_dhm_calc_secret(&dhm_srv, srv_secret, sizeof(srv_secret), 
                                    &n, mbedtls_ctr_drbg_random, &ctr_drbg);
    assert_exit(ret == 0, ret);
    dump_buf("  6. dh server generate secret:", srv_secret, sizeof(srv_secret));

    ret = mbedtls_dhm_calc_secret(&dhm_cli, cli_secret, sizeof(cli_secret), 
                                    &n, mbedtls_ctr_drbg_random, &ctr_drbg);
    assert_exit(ret == 0, ret);
    dump_buf("  7. dh client generate secret:", cli_secret, sizeof(cli_secret));

    ret = memcmp(cli_secret, srv_secret, sizeof(srv_secret));
    assert_exit(ret == 0, ret);
    mbedtls_printf("  8. dh checking secrets ... ok\n\n");

cleanup:
    mbedtls_dhm_free(&dhm_cli); 
    mbedtls_dhm_free(&dhm_srv);
    mbedtls_entropy_free(&entropy); 
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return ret;    
}
