cmake_minimum_required(VERSION 3.10)
project(fire)

set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
add_executable(run_tests tests/tests.cpp fire.hpp)
target_link_libraries(run_tests GTest::GTest GTest::Main)
gtest_discover_tests(run_tests)
add_test(NAME monolithic COMMAND run_tests)
set_target_properties(run_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_executable(link_test tests/link_func.cpp tests/link_main.cpp)
set_target_properties(link_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

configure_file(tests/run_examples.py tests/run_examples.py COPYONLY)
configure_file(tests/run_all_tests.py tests/run_all_tests.py COPYONLY)



add_executable(all_combinations examples/all_combinations.cpp fire.hpp)
set_target_properties(all_combinations PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(basic examples/basic.cpp fire.hpp)
set_target_properties(basic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(flags examples/flags.cpp fire.hpp)
set_target_properties(flags PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(positional examples/positional.cpp fire.hpp)
set_target_properties(positional PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(vector_positional examples/vector_positional.cpp fire.hpp)
set_target_properties(vector_positional PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
