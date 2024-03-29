cmake_minimum_required(VERSION 3.1)

if(${CMAKE_VERSION} VERSION_GREATER "3.11.0")
    include(CTest)
    include(FetchContent)
    include(GoogleTest)

    enable_testing()

    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/release-1.11.0.tar.gz
            URL_HASH SHA3_224=78cfdd81a73fa617bcf0e5f13d9d825c6ea069106757fb51cc8120eb
    )

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
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
