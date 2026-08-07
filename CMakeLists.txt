cmake_minimum_required(VERSION 3.16)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/camd.h" CAMD_VERSION_LINES
    REGEX "^#define CAMD_(MAIN|SUB|SUBSUB)_VERSION")
foreach(component MAIN SUB SUBSUB)
    string(REGEX MATCH "CAMD_${component}_VERSION[ 	]+([0-9]+)" match
        "${CAMD_VERSION_LINES}")
    if(NOT match)
        message(FATAL_ERROR "Cannot determine CAMD ${component} version")
    endif()
    set(CAMD_VERSION_${component} "${CMAKE_MATCH_1}")
endforeach()
set(CAMD_VERSION
    "${CAMD_VERSION_MAIN}.${CAMD_VERSION_SUB}.${CAMD_VERSION_SUBSUB}")

project(CAMD VERSION "${CAMD_VERSION}" LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(CAMD_BUILD_TESTS "Build the upstream CAMD demo tests" ${PROJECT_IS_TOP_LEVEL})

file(GLOB CAMD_UPSTREAM_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/upstream/*.c")

add_library(camd
    ${CAMD_UPSTREAM_SOURCES}
    src/suitesparse_config_compat.c)
add_library(CAMD::CAMD ALIAS camd)
add_library(SuiteSparse::CAMD ALIAS camd)

set_target_properties(camd PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    OUTPUT_NAME camd
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    EXPORT_NAME CAMD)

target_include_directories(camd
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/upstream")

if(NOT WIN32)
    target_link_libraries(camd PRIVATE m)
endif()

if(CAMD_BUILD_TESTS)
    include(CTest)
    foreach(demo camd_demo camd_l_demo camd_demo2 camd_simple)
        add_executable(${demo} "tests/upstream/${demo}.c")
        target_link_libraries(${demo} PRIVATE camd)
        add_test(NAME upstream_${demo}
            COMMAND "${CMAKE_COMMAND}"
                -DPROGRAM=$<TARGET_FILE:${demo}>
                -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/tests/upstream/${demo}.out
                -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${demo}.out
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RunAndCompare.cmake)
    endforeach()
    add_test(NAME installed_package
        COMMAND "${CMAKE_COMMAND}"
            -DBUILD_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -DINSTALL_CONFIG=$<CONFIG>
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/TestInstall.cmake)
endif()

install(TARGETS camd
    EXPORT CAMDTargets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES
    include/camd.h
    include/SuiteSparse_config.h
    include/SuiteSparse_config_version.h
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(EXPORT CAMDTargets
    FILE CAMDTargets.cmake
    NAMESPACE CAMD::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CAMD")

configure_package_config_file(
    cmake/CAMDConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/CAMDConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CAMD")
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CAMDConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CAMDConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CAMDConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CAMD")

set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "${CMAKE_INSTALL_PREFIX}")
set(libdir "${CMAKE_INSTALL_FULL_LIBDIR}")
set(includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
configure_file(cmake/camd.pc.in camd.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/camd.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
