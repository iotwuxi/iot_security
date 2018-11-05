#include <stdio.h>
#include "mbedtls/ssl.h"
#include "mbedtls/platform.h"

int main( void )
{
    int index = 1;
    const int *list;
    const char *name;

    mbedtls_printf("\n  Available Ciphersuite:\n");
    list = mbedtls_ssl_list_ciphersuites();
    for(; *list; list++) {
        name = mbedtls_ssl_get_ciphersuite_name(*list);
        mbedtls_printf("  [%03d] %s\n", index++, name);
    }
    mbedtls_printf("\n");
    
    return 0;
}
