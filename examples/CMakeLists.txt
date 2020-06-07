cmake_minimum_required(VERSION 3.1)

add_executable(all_combinations all_combinations.cpp ../fire.hpp)
add_executable(basic basic.cpp ../fire.hpp)
add_executable(flag flag.cpp ../fire.hpp)
add_executable(optional_and_default optional_and_default.cpp ../fire.hpp)
add_executable(positional positional.cpp ../fire.hpp)
add_executable(vector_positional vector_positional.cpp ../fire.hpp)

set(EXAMPLES_BUILD_DIR $<TARGET_FILE_DIR:basic> PARENT_SCOPE)
