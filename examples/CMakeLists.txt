cmake_minimum_required(VERSION 3.1)

add_executable(all_combinations all_combinations.cpp)
target_link_libraries(all_combinations fire-hpp)

add_executable(basic basic.cpp)
target_link_libraries(basic fire-hpp)

add_executable(flag flag.cpp)
target_link_libraries(flag fire-hpp)

add_executable(optional_and_default optional_and_default.cpp)
target_link_libraries(optional_and_default fire-hpp)

add_executable(positional positional.cpp)
target_link_libraries(positional fire-hpp)

add_executable(variadic variadic.cpp)
target_link_libraries(variadic fire-hpp)

if(MSVC)
    # FIXIT: Test exception handling on Windows too
else()
    add_executable(no_exceptions no_exceptions.cpp)
    target_link_libraries(no_exceptions fire-hpp)
    target_compile_options(no_exceptions PRIVATE -fno-exceptions)
endif()

set(EXAMPLES_BUILD_DIR $<TARGET_FILE_DIR:basic> PARENT_SCOPE)
