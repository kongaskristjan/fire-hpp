
## CMake usage

Fire can easily be used in other C++ CMake projects.

<a id="submodule"></a> You may use Fire from a folder in your project (typically a git submodule).

```cmake
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(foo)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(fire_folder)

add_executable(bar bar.cpp)
target_link_libraries(bar fire-hpp::fire-hpp)
```

<a id="fetchcontent"></a> Alternatively, you can also use the more modern FetchContent, which automatically downloads fire-hpp from github.

```cmake
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(foo)
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  fire
  GIT_REPOSITORY https://github.com/kongaskristjan/fire-hpp
)
FetchContent_MakeAvailable(fire)

add_executable(bar bar.cpp)
target_link_libraries(bar fire)
```

<a id="find_package"></a> Fire can also be installed and found through `find_package()`:

```cmake
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(foo)
set(CMAKE_CXX_STANDARD 11)

find_package(fire-hpp REQUIRED)
add_executable(bar bar.cpp)
target_link_libraries(bar fire-hpp::fire-hpp)
```
