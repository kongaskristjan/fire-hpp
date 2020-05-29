cmake_minimum_required(VERSION 3.1)
project(fire)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(link_test tests/link_func.cpp tests/link_main.cpp)
set_target_properties(link_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

configure_file(tests/run_examples.py tests/run_examples.py COPYONLY)

find_package(GTest)
if(GTest_FOUND AND ${CMAKE_VERSION} VERSION_GREATER "3.10.0")
    add_executable(run_tests tests/tests.cpp fire.hpp)
    target_link_libraries(run_tests GTest::GTest GTest::Main)
    gtest_discover_tests(run_tests)
    add_test(NAME monolithic COMMAND run_tests)
    set_target_properties(run_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
    configure_file(tests/run_all_tests.py tests/run_all_tests.py COPYONLY)
endif()



add_executable(all_combinations examples/all_combinations.cpp fire.hpp)
set_target_properties(all_combinations PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(basic examples/basic.cpp fire.hpp)
set_target_properties(basic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(flag examples/flag.cpp fire.hpp)
set_target_properties(flag PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(optional_and_default examples/optional_and_default.cpp fire.hpp)
set_target_properties(optional_and_default PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(positional examples/positional.cpp fire.hpp)
set_target_properties(positional PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)

add_executable(vector_positional examples/vector_positional.cpp fire.hpp)
set_target_properties(vector_positional PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
