cmake_minimum_required(VERSION 3.8)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

if (CONFIG_MBEDTLS)
target_include_directories(mbedTLS INTERFACE ${PROJECT_SOURCE_DIR}/src)
endif()

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

