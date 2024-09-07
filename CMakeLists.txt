cmake_minimum_required(VERSION 3.5)
project(fire-hpp)

include(GNUInstallDirs)

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
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -pedantic -Werror)
    endif()
endif()
set(ignoreMe "${DISABLE_PEDANTIC}")

# Setup Fire to be used in 3rd party project using exports. Create a fire target
add_library(fire-hpp INTERFACE)
target_compile_features(fire-hpp INTERFACE cxx_std_11)
target_include_directories(fire-hpp INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS fire-hpp EXPORT fire-hpp-targets)
install(EXPORT fire-hpp-targets
    FILE fire-hpp-config.cmake
    NAMESPACE fire-hpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fire-hpp)

# check if we're included as a subproject (i.e. through add_subdirectory()
get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)
if (IS_SUBPROJECT)
    message(STATUS "fire-hpp included as a subproject.")
    # add export alias to mirror find_package()-exported target
    if (NOT TARGET fire-hpp::fire-hpp)
        message(STATUS "Adding 'fire-hpp::fire-hpp' export alias.")
        add_library(fire-hpp::fire-hpp ALIAS fire-hpp)
    endif()

    # Disable tests and examples
    set(FIRE_EXAMPLES FALSE)
    set(FIRE_UNIT_TESTS FALSE)
endif()

if (FIRE_EXAMPLES)
    add_subdirectory(examples)
endif()

if (FIRE_UNIT_TESTS)
    add_subdirectory(tests)
endif()
