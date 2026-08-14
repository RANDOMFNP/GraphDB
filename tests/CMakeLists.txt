project(graphlib_tests LANGUAGES CXX)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests PRIVATE graphlib)

add_test(NAME Tests COMMAND run_tests)