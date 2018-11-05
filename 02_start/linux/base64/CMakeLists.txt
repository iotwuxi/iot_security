cmake_minimum_required(VERSION 3.8.2)
project("Base64")

include_directories(./ $ENV{MBEDTLS_BASE}/include)
aux_source_directory($ENV{MBEDTLS_BASE}/library MBEDTLS_SOURCES)

set(SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/base64.c 
	${MBEDTLS_SOURCES})

add_executable(base64 ${SOURCES})
