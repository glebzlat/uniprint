project(uniprint_test LANGUAGES CXX)

include(CTest)

# list of all test targets as a dependency for a test launcher target
set(EXEC_LIST)

macro(add_test_exec TEST_NAME DEPLIB)
    add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${ARGN})
    target_link_libraries(${TEST_NAME} PRIVATE ${DEPLIB})
    list(APPEND EXEC_LIST ${TEST_NAME})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro()

add_test_exec(GFP_Test uniprint gfp_test.cpp)
add_test_exec(Uniprint uniprint uniprint.cpp)

add_custom_target(MakeTest ALL
    ctest --output-on-failure --test-timeout 10
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    USES_TERMINAL
    VERBATIM
    DEPENDS ${EXEC_LIST}
)
