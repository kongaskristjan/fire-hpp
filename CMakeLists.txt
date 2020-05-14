cmake_minimum_required(VERSION 3.10)
project(fire)

set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
add_executable(run_tests tests.cpp fire.h)
target_link_libraries(run_tests GTest::GTest GTest::Main)
gtest_discover_tests(run_tests)
add_test(NAME monolithic COMMAND run_tests)

add_executable(manual_test main.cpp fire.h)
