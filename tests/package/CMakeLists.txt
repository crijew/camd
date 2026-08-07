cmake_minimum_required(VERSION 3.16)
project(CAMDPackageConsumer LANGUAGES C)

find_package(CAMD CONFIG REQUIRED)

add_executable(camd_package_consumer main.c)
target_link_libraries(camd_package_consumer PRIVATE CAMD::CAMD)

add_executable(camd_suitesparse_alias_consumer main.c)
target_link_libraries(camd_suitesparse_alias_consumer PRIVATE SuiteSparse::CAMD)
