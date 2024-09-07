cmake_minimum_required(VERSION 3.5)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER "3.14.0")
    include(CTest)
    include(FetchContent)
    include(GoogleTest)

    enable_testing()

    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
            URL_HASH SHA3_224=f3d058b3e9a9c1eb53c40e8fd9adfc31047a7eb7189d4e4e49647ef0
    )

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_MakeAvailable(googletest)
    endif()

    add_executable(run_tests tests.cpp)
    target_link_libraries(run_tests fire-hpp gtest gtest_main)
    gtest_discover_tests(run_tests)

    configure_file(run_standard_tests.py run_standard_tests.py COPYONLY)

    set(RUN_TESTS_BUILD_DIR $<TARGET_FILE_DIR:run_tests>)
    add_custom_command(TARGET run_tests
            COMMAND ${CMAKE_COMMAND}
            -D RUN_TESTS_BUILD_DIR=${RUN_TESTS_BUILD_DIR}
            -D EXAMPLES_BUILD_DIR=${EXAMPLES_BUILD_DIR}
            -D FILE_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P "${CMAKE_CURRENT_SOURCE_DIR}/create_build_dirs_txt.cmake"
    )
endif()

configure_file(run_examples.py run_examples.py COPYONLY)

add_executable(link_test link_func.cpp link_main.cpp)
target_link_libraries(link_test fire-hpp)
