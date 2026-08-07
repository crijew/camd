if(NOT DEFINED BUILD_DIR OR NOT DEFINED SOURCE_DIR)
    message(FATAL_ERROR "BUILD_DIR and SOURCE_DIR are required")
endif()

set(install_dir "${BUILD_DIR}/test-install")
set(consumer_dir "${BUILD_DIR}/test-consumer")
file(REMOVE_RECURSE "${install_dir}" "${consumer_dir}")

set(config_args)
if(DEFINED INSTALL_CONFIG AND NOT INSTALL_CONFIG STREQUAL "")
    list(APPEND config_args --config "${INSTALL_CONFIG}")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --install "${BUILD_DIR}"
        ${config_args} --prefix "${install_dir}"
    RESULT_VARIABLE result)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "CAMD installation failed")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}"
        -S "${SOURCE_DIR}/tests/package"
        -B "${consumer_dir}"
        -DCMAKE_PREFIX_PATH=${install_dir}
    RESULT_VARIABLE result)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Installed CAMD package configuration failed")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${consumer_dir}" ${config_args}
    RESULT_VARIABLE result)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Installed CAMD package consumer failed to build")
endif()
