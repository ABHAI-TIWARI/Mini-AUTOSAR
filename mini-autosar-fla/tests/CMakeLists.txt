cmake_minimum_required(VERSION 3.16)

# ─── Minimal test framework bundled with project ────────────────────
# No external dependencies (no Unity/Ceedling required to run).
# Each test file is a standalone executable.

set(TEST_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src/mcal
    ${CMAKE_SOURCE_DIR}/src/bsw
    ${CMAKE_SOURCE_DIR}/src/rte
    ${CMAKE_SOURCE_DIR}/src/app
    ${CMAKE_SOURCE_DIR}/tests
)

# Helper macro to register a test executable
macro(add_autosar_test test_name test_file)
    add_executable(${test_name} ${test_file} minitest.c)
    target_include_directories(${test_name} PRIVATE ${TEST_INCLUDE_DIRS})
    target_link_libraries(${test_name} app rte bsw mcal)
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

add_autosar_test(test_mcal_dio       test_Mcal_Dio.c)
add_autosar_test(test_button_reader  test_ButtonReader.c)
add_autosar_test(test_blink_ctrl     test_BlinkController.c)
add_autosar_test(test_heartbeat      test_Heartbeat.c)
add_autosar_test(test_det            test_Det.c)
