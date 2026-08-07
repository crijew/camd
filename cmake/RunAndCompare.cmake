if(NOT DEFINED PROGRAM OR NOT DEFINED EXPECTED OR NOT DEFINED OUTPUT)
    message(FATAL_ERROR "PROGRAM, EXPECTED, and OUTPUT are required")
endif()

execute_process(
    COMMAND "${PROGRAM}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE actual
    ERROR_VARIABLE stderr)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "${PROGRAM} failed (${result}):\n${stderr}")
endif()

file(READ "${EXPECTED}" expected)
string(REPLACE "\r\n" "\n" actual "${actual}")
string(REPLACE "\r\n" "\n" expected "${expected}")
file(WRITE "${OUTPUT}" "${actual}")
if(NOT actual STREQUAL expected)
    execute_process(COMMAND "${CMAKE_COMMAND}" -E compare_files
        "${EXPECTED}" "${OUTPUT}")
    message(FATAL_ERROR "Output differs from ${EXPECTED}; actual: ${OUTPUT}")
endif()
