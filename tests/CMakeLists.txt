cmake_minimum_required(VERSION 3.1)

if(${CMAKE_VERSION} VERSION_GREATER "3.14.0")
    include(CTest)
    include(FetchContent)
    include(GoogleTest)

    enable_testing()

    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.10.0
    )
    FetchContent_MakeAvailable(googletest)  # (3)

    add_executable(run_tests tests.cpp ../fire.hpp)  # (4)
    target_link_libraries(run_tests gmock gmock_main)
    gtest_discover_tests(run_tests)

    configure_file(run_standard_tests.py run_standard_tests.py COPYONLY)
endif()

configure_file(run_examples.py run_examples.py COPYONLY)

add_executable(link_test link_func.cpp link_main.cpp)
