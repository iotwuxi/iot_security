cmake_minimum_required(VERSION 3.8.2)
project("Ciphersuite-list")

include_directories(./ $ENV{MBEDTLS_BASE}/include)
#add_definitions(-DMBEDTLS_CONFIG_FILE=<$ENV{MBEDTLS_BASE}/configs/config-mini-tls1_1.h>)
aux_source_directory($ENV{MBEDTLS_BASE}/library MBEDTLS_SOURCES)

set(SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/ciphersuite-list.c 
	${MBEDTLS_SOURCES})

add_executable(ciphersuite-list ${SOURCES})
