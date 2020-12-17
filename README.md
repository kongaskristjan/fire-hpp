
# Fire for C++

Fire for C++, inspired by [python-fire](https://github.com/google/python-fire), is a single header library that creates a command line interface from a function signature. Here's the whole program for adding two numbers with command line:
 ```
#include <iostream>
#include <fire-hpp/fire.hpp>

int fired_main(int x = fire::arg("-x"), int y = fire::arg("-y")) {
    std::cout << x + y << std::endl;
    return 0;
}

FIRE(fired_main)
```

That's all. Usage:

```
$ ./add -x=1 -y=2
3
```

As you likely expect,
* `--help` prints a meaningful message with required arguments and their types.
* an error message is displayed for incorrect usage.
* the program runs on Linux, Windows and Mac OS.

See [examples](https://github.com/kongaskristjan/fire-hpp/tree/master/examples) for other kinds of arguments.

### Why yet another CLI library?!

With most libraries, creating a CLI roughly follows this pattern:
1. define arguments
2. call `parse(argc, argv);`
3. check whether errors are detected by `parse()`, print them and return (optional)
4. check for `-h` and `--help`, print the help message and return (optional)
5. for each argument:
    1. get argument from the map and if necessary convert to the right type
    2. try/catch for errors in conversion and return (optional)

That's a non-trivial amount of boilerplate, especially for simple scripts. Because of that, programmers (and a lot of library examples) tend to skip the optional parts, however this incurs a significant usability cost. Also, many libraries don't help at all with the conversion step.

With fire-hpp, you only call `FIRE(fired_main)` and define arguments as function parameters. When `fired_main()` scope begins, all steps have already been completed.

### What's covered?

* [flags](#flag); [named and positional](#identifier) parameters; [variadic parameters](#variadic)
* [optional parameters](#optional)/[default values](#default)
* conversions to [integer, floating-point and `std::string`](#standard)
* [program](#fire)/[parameter](#description) descriptions
* standard constructs, such as `-abc <=> -a -b -c` and `-x=1 <=> -x 1`

## Q. Quickstart

### Q.1 Requirements

* Using fire.hpp: C++11 compatible compiler.
* Compiling examples: CMake 3.1+.
* Compiling/running tests: CMake 3.11+ and Python 3.5+. GTest is downloaded, compiled and linked automatically.

### Q.2 Running examples

Steps to run examples:
* Clone repo: `git clone https://github.com/kongaskristjan/fire-hpp`
* Create build and change directory: `cd fire-hpp && mkdir build && cd build`
* Configure/build: `cmake .. && cmake --build .` (or substitute the latter command with appropriate build system invocation, eg. `make -j8` or `ninja`)
* If errors are encountered, clear the build directory and disable pedantic warnings as errors with `cmake -D DISABLE_PEDANTIC= ..` (you are encouraged to open an issue).
* Run: `./examples/basic --help` or `./examples/basic -x=3 -y=5`

## T. Tutorial

Let's go through each part of the following example.

```
int fired_main(int x = fire::arg("-x"), int y = fire::arg("-y")) { // Define and convert arguments
    std::cout << x + y << std::endl; // Use x and y, they're ints!
    return 0;
}

FIRE(fired_main) // call fired_main()
```

* __FIRE(function name)__
`FIRE(fired_main)` expands into the actual `main()` function that defines your program's entry point and fires off `fired_main()`. `fired_main` is called without arguments, thus compiler is forced to use the default `fire::arg` values.

* __fire::arg(identifiers[, default value])__
 A constructor that accepts the name/shorthand/description/position of the argument. Use a brace enclosed list for several of them (eg. `fire::arg({"-x", "--longer-name", "description of the argument"})` or `fire::arg({0, "zeroth element"})`. The library expects a single dash for single-character shorthands, two dashes for multi-character names, and zero dashes for descriptions. `fire::arg` objects should be used as default values for fired function parameters. See [documentation](#fire_arg) for more info.

* __int fired_main(arguments)__
This is what you perceive as the program entry point. All arguments must be `bool`, integral, floating-point, `fire::optional<T>`, `std::string` or `std::vector<T>` type and default initialized with `fire::arg` objects (Failing to initialize properly results in undefined behavior!). See [conversions](#conversions) to learn how each of them affects the CLI.

## D. Documentation

### <a id="fire"></a> D.1 FIRE(fired_main[, program_description]) and variants

* `FIRE(fired_main[, program_description])` creates the main function that parses arguments and calls `fired_main`.
* `FIRE_NO_EXCEPTIONS(...)` is similar, but can be used even if compiler has exceptions disabled. However, this imposes limitations on what the library can parse. Specifically, it disallows space assignment, eg. `-x 1` must be written as `-x=1`.
* `FIRE_ALLOW_UNUSED(...)` is similar to `FIRE(...)`, but allows unused arguments. This is useful when [raw arguments](#raw_args) are accessed (eg. for another library).

Program description can be supplied as the second argument:
```
FIRE(fired_main, "Hello there")
```


### D.2 <a id="fire_arg"></a> fire::arg(identifiers[, default_value]])

#### <a id="identifier"></a> D.2.1 Identifiers

Identifiers are used to find arguments from command line and provide a description. In general, it's a brace enclosed list of elements (braces can be omitted for a single element):
* `"-s"` shorthand name for argument
* `"--multicharacter-name"`
* `0` index of a positional argument
* `"<name of the positional argument>"`
* any other string: `"description of any argument"`
* variadic arguments: `fire::variadic()`

--------

* Example: `int fired_main(int x = fire::arg("-x"));`
    * CLI usage: `program -x=1`


* Example: `int fired_main(int x = fire::arg({"-x", "--long-name"}));`
    * CLI usage: `program -x=1`
    * CLI usage: `program --long-name=1`


* Example: `int fired_main(int x = fire::arg({0, "<name of argument>", "description"}));`
    * CLI usage: `program 1`
    * `<name of argument>` and `description` appear in help messages


* Example: `int fired_main(vector<int> x = fire::arg(fire::variadic()));`
    * CLI usage: `program 1 2 3`

#### <a id="default"></a> D.2.2 Default value (optional)

Default value if no value is provided through command line. Can be either `std::string`, integral or floating-point type and `fire::arg` must be converted to that same type. This default is also displayed on the help page.

* Example: `int fired_main(int x = fire::arg({"-x", "--long-name"}, 0));`
    * CLI usage: `program` -> `x==0`
    * CLI usage: `program -x=1` -> `x==1`

For an optional argument without a default, see [fire::optional](#optional).

### <a id="conversions"></a> D.3 fire::arg conversions

To conveniently obtain arguments with the right type and automatically check the validity of input, `fire::arg` class defines several implicit conversions.

#### <a id="standard"></a> D.3.1 std::string, integral, or floating point

Converts the argument value on command line to the respective type. Displays an error if the conversion is impossible or default value has wrong type.

* Example: `int fired_main(std::string name = fire::arg("--name"));`
    * CLI usage: `program --name=fire` -> `name=="fire"`


* Example: `int fired_main(double x = fire::arg("-x"));`
    * CLI usage: `program -x=2.5` -> `x==2.5`
    * CLI usage: `program -x=blah` -> `Error: value blah is not a real number`

#### <a id="optional"></a> D.3.2 fire::optional

Used for optional arguments without a reasonable default value. This way the default value doesn't get printed in a help message. The underlying type can be `std::string`, integral or floating-point.

`fire::optional` is a tear-down version of [`std::optional`](https://en.cppreference.com/w/cpp/utility/optional), with compatible implementations for [`has_value()`](https://en.cppreference.com/w/cpp/utility/optional/operator_bool), [`value_or()`](https://en.cppreference.com/w/cpp/utility/optional/value_or) and [`value()`](https://en.cppreference.com/w/cpp/utility/optional/value).

* Example: `int fired_main(fire::optional<std::string> name = fire::arg("--name"));`
    * CLI usage: `program` -> `name.has_value()==false`, `name.value_or("default")=="default"`
    * CLI usage: `program --name="fire"` -> `name.has_value()==true` and `name.value_or("default")==name.value()=="fire"`

#### <a id="flag"></a> D.3.3 bool: flag argument

Boolean flags are `true` when they exist on command line and `false` when they don't. Multiple single-character flags can be packed on command line by prefixing with a single hyphen: `-abc <=> -a -b -c`

* Example: `int fired_main(bool flag = fire::arg("--flag"));`
    * CLI usage: `program` -> `flag==false`
    * CLI usage: `program --flag` -> `flag==true`

#### <a id="variadic"></a> D.3.4 std::vector<T>: variadic argument

A method for getting all positional arguments as a vector. The `fire::arg` object can be converted to `std::vector<std::string>`, `std::vector<integral type>` or `std::vector<floating-point type>`. Using variadic argument forbids extracting positional arguments with `fire::arg(index)`.

In this case, identifier should be `fire::variadic()`. Description can be supplied in the usual way.

* Example: `int fired_main(vector<std::string> params = fire::arg({fire::variadic(), "description"}));`
    * CLI usage: `program abc xyz` -> `params=={"abc", "xyz"}`
    * CLI usage: `program` -> `params=={}`

### <a id="raw_args"></a>  D.4 Accessing raw arguments

Some libraries require access to raw argc/argv - this is accomplished with `fire::original_args`:

```
int argc = fire::original_args.argc();
const char ** argv = fire::original_args.argv();
```

In such case [`FIRE_ALLOW_UNUSED(...)`](#fire) is probably what you want.

## CMake integration

Fire can easily be used by other C++ CMake projects.

You may use Fire from a folder in your project (typically a git submodule).

```
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(foo)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(fire_folder)

add_executable(bar bar.cpp)
target_link_libraries(bar fire-hpp::fire-hpp)
```

Alternatively, you can also use the more modern FetchContent.

```
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

Fire can also be installed and found through `find_package()`:

```
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(foo)
set(CMAKE_CXX_STANDARD 11)

find_package(fire-hpp REQUIRED)
add_executable(bar bar.cpp)
target_link_libraries(bar fire-hpp::fire-hpp)
```

## Conan integration

Fire can be packaged for consumption through Conan by running `conan create . fire-hpp/version@user/channel` from the root directory of this repository. It can then be consumed as in the `find_package()` example above if using `cmake_find_package` generator.

## Development

This library uses extensive testing. Unit tests are located in `tests/`, while `examples/` are used as integration tests. The latter also ensures examples are up-to-date. Before committing, please verify `python3 ./build/tests/run_standard_tests.py` succeed. Releases are also tested on many platforms with `python3 ./tests/run_release_tests.py`.

v0.2 release is tested on:
* Arch Linux: gcc==10.2.0, clang==10.0.1: C++11, C++14, C++17, C++20
* Ubuntu 18.04: gcc=={4.8, 4.9}: C++11 and gcc=={5.5, 6.5, 7.5, 8.4}: C++11, C++14, C++17
* Ubuntu 18.04: clang=={3.5, 3.6, 3.7, 3.8, 3.9, 4.0}: C++11, C++14 and clang=={5.0, 6.0, 7.0, 8.0, 9.0}: C++11, C++14, C++17
* Windows 10: MSVC=={19.26} (2019 Build Tools): C++11, C++14, C++17
* Mac OS: XCode=={12.0} (requires CMake 3.18.1+): C++11, C++14, C++17

### Roadmap:

#### Current state

#### v0.2 release

* Sufficient information for contributing
    * Create a document describing the internals of this project
    * Document the code
    * Add contribution guidelines
    * Add issue and PR templates
    * Thoroughly describe each task in the roadmap
* Subcommands (eg. `git add` and `git show`, which may have different flags/options)
* Possibility to raise errors and print help in fired_main()
* Host documentation on readthedocs.io

#### v0.3 release

* Solve Windows non-ascii character input
* Self-defined objects with string streams
* Argument requirements
    * `fire::arg(identifier).require(condition, msg)`
    * Specialized requirements, eg. `bounds(min, max)`, `one_of([option1], [option2], ...)`
* `fire::arg(identifier).save([condition])`, which will save the program from exiting even if not all required arguments are present or correct (eg. for `--version`)

#### v0.4 release

* Ensure API user gets an error message when using required positional arguments after optional positional arguments
* Allow converting named `fire::arg` to `std::vector` or `std::map` (allows multiple occurrences of an argument, like cmake's `-D`)
* Automatic testing for error messages

#### v1.0 release
