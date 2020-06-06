cmake_minimum_required(VERSION 3.1)
project(fire)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


add_subdirectory(tests)

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
