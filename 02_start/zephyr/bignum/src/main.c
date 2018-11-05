#include <zephyr.h>

#include <string.h>
#include <stdio.h>

#include "mbedtls/bignum.h"
#include "mbedtls/platform.h"

static void dump_buf(char *buf, size_t len) 
{
    for (int i = 0; i < len; i++) {
        mbedtls_printf("%c%s", buf[i], 
                        (i + 1) % 32 ? "" : "\n\t"); 
    }
    mbedtls_printf("\n");
}

int main(void)
{
    size_t olen;
    char buf[256];
    mbedtls_mpi A, E, N, X;

    mbedtls_platform_set_printf(printf);

    mbedtls_mpi_init(&A); 
    mbedtls_mpi_init(&E); 
    mbedtls_mpi_init(&N); 
    mbedtls_mpi_init(&X);

    mbedtls_mpi_read_string(&A, 16,
        "EFE021C2645FD1DC586E69184AF4A31E" \
        "D5F53E93B5F123FA41680867BA110131" \
        "944FE7952E2517337780CB0DB80E61AA" \
        "E7C8DDC6C5C6AADEB34EB38A2F40D5E6" );

    mbedtls_mpi_read_string(&E, 16,
        "B2E7EFD37075B9F03FF989C7C5051C20" \
        "34D2A323810251127E7BF8625A4F49A5" \
        "F3E27F4DA8BD59C47D6DAABA4C8127BD" \
        "5B5C25763222FEFCCFC38B832366C29E" );

    mbedtls_mpi_read_string(&N, 16,
        "0066A198186C18C10B2F5ED9B522752A" \
        "9830B69916E535C8F047518A889A43A5" \
        "94B6BED27A168D31D4A52F88925AA8F5" );

    mbedtls_mpi_mul_mpi(&X, &A, &N);
    mbedtls_mpi_write_string(&X, 16, buf, 256, &olen);
    mbedtls_printf("\n  X = A * N = \n\t");
    dump_buf(buf, olen);

    mbedtls_mpi_exp_mod(&X, &A, &E, &N, NULL);
    mbedtls_mpi_write_string(&X, 16, buf, 256, &olen);
    mbedtls_printf("\n  X = A^E mode N = \n\t");
    dump_buf(buf, olen);

    mbedtls_mpi_inv_mod( &X, &A, &N);
    mbedtls_mpi_write_string(&X, 16, buf, 256, &olen);
    mbedtls_printf("\n  X = A^-1 mod N = \n\t");
    dump_buf(buf, olen);

    mbedtls_mpi_free(&A); 
    mbedtls_mpi_free(&E);
    mbedtls_mpi_free(&N); 
    mbedtls_mpi_free(&X);

    return 0;   
}