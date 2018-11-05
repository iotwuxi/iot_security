#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

/* System support */
#define MBEDTLS_PLATFORM_ASM
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_MEMORY_BUFFER_ALLOC_C
#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS
#define MBEDTLS_PLATFORM_EXIT_ALT
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#define MBEDTLS_PLATFORM_PRINTF_ALT

/* mbed TLS feature support */
#define MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_PROTO_DTLS

/* mbed TLS modules */
#define MBEDTLS_AES_C
#define MBEDTLS_CCM_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_MD_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_SRV_C
#define MBEDTLS_SSL_TLS_C

/* Save RAM at the expense of ROM */
#define MBEDTLS_AES_ROM_TABLES

/* Save some RAM by adjusting to your exact needs */
#define MBEDTLS_PSK_MAX_LEN    16 /* 128-bits keys are generally enough */

/*
 * You should adjust this to the exact number of sources you're using: default
 * is the "platform_entropy_poll" source, but you may want to add other ones
 * Minimum is 2 for the entropy test suite.
 */
#define MBEDTLS_ENTROPY_MAX_SOURCES 2

/*
 * Use only CCM_8 ciphersuites, and
 * save ROM and a few bytes of RAM by specifying our own ciphersuite list
 */
#define MBEDTLS_SSL_CIPHERSUITES  MBEDTLS_TLS_PSK_WITH_AES_128_CCM_8

/*
 * Save RAM at the expense of interoperability: do this only if you control
 * both ends of the connection!  (See comments in "mbedtls/ssl.h".)
 * The optimal size here depends on the typical size of records.
 */
#define MBEDTLS_SSL_MAX_CONTENT_LEN             512

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */
