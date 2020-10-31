cmake_minimum_required(VERSION 3.1)
project(fire-hpp)

option(FIRE_EXAMPLES "Compile examples" ON)
option(FIRE_UNIT_TESTS "Enable unit tests" ON)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

if(NOT DEFINED DISABLE_PEDANTIC)
    if(MSVC)
        add_compile_options(/W4 /WX)
    else()
        add_compile_options(-Wall -Wextra -pedantic -Werror)
    endif()
endif()
set(ignoreMe "${DISABLE_PEDANTIC}")

# Setup Fire to be used in 3rd party project using exports. Create a fire target
add_library(fire-hpp INTERFACE)
target_include_directories(fire-hpp INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/include")

# Disable examples and test if fire is a sub-project
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(FIRE_EXAMPLES FALSE)
    set(FIRE_UNIT_TESTS FALSE)
endif()

if (FIRE_EXAMPLES)
    add_subdirectory(examples)
endif()

if (FIRE_UNIT_TESTS)
    add_subdirectory(tests)
endif()