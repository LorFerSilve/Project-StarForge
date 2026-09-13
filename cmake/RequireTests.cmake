if(NOT DEFINED STARFORGE_TEST_BUILD_DIR)
    message(FATAL_ERROR "STARFORGE_TEST_BUILD_DIR is required")
endif()

if(NOT DEFINED STARFORGE_TEST_LABEL)
    set(STARFORGE_TEST_LABEL "unit")
endif()

if(NOT CMAKE_CTEST_COMMAND)
    find_program(CMAKE_CTEST_COMMAND ctest REQUIRED)
endif()

execute_process(
    COMMAND "${CMAKE_CTEST_COMMAND}" --test-dir "${STARFORGE_TEST_BUILD_DIR}" -C Debug -N -L "${STARFORGE_TEST_LABEL}"
    RESULT_VARIABLE discovery_result
    OUTPUT_VARIABLE discovery_output
    ERROR_VARIABLE discovery_error
)

if(NOT discovery_result EQUAL 0)
    message(FATAL_ERROR "CTest discovery failed: ${discovery_error}")
endif()

string(REGEX MATCH "Total Tests: ([0-9]+)" test_count_match "${discovery_output}")
if(NOT test_count_match)
    message(FATAL_ERROR "Could not determine CTest discovery count. Output:\n${discovery_output}")
endif()

set(test_count "${CMAKE_MATCH_1}")
if(test_count LESS 1)
    message(FATAL_ERROR "Required CTest label '${STARFORGE_TEST_LABEL}' discovered zero tests")
endif()

message(STATUS "Required CTest label '${STARFORGE_TEST_LABEL}' discovered ${test_count} test(s)")
