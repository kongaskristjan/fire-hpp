
# Fire for C++

Fire for C++, inspired by [python-fire](https://github.com/google/python-fire), is a library that creates a command line interface from a function signature. Here's a program for adding two numbers in command line:
 ```
#include <iostream>
#include <fire.hpp>

int fired_main(int x = fire::arg("x"), int y = fire::arg("y")) {
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

### Why yet another CLI library?!

With most libraries, creating a CLI roughly follows this pattern:
1. define arguments and call `parse(argc, argv)`
2. check whether errors are detected by `parse()`, print them and return
3. check for `-h` and `--help`, print the help message and return
4. for each argument:
    1. get argument from the map and convert to the right type
    2. try/catch for errors in conversion and return

That's a non-trivial amount of boilerplate, especially for simple scripts. It's possible to get away without error checking and help messages, however that comes with significant usability cost. Also, many libraries don't help at all with the conversion step and this is usually skipped in examples too.

With fire-hpp, you only call `FIRE(...)` and define arguments. When `fired_main()` scope begins, all four steps have already been completed.

### What's covered?

* [flags](#flag); [named and positional](#identifier) parameters; [variable number of parameters](#vector)
* [optional parameters](#optional)/[default values](#default)
* conversions to [integer, floating-point and `std::string`](#standard)
* [parameter descriptions](#description)
* typical constructs, such as expanding `-abc <=> -a -b -c` and `-x=1 <=> -x 1`

In addition, this library
* works with Linux, Windows and Mac OS
* is a single header
* comes under very permissive [Boost licence](https://choosealicense.com/licenses/bsl-1.0/) (examples with [0-clause BSD](https://choosealicense.com/licenses/0bsd/))

## Q. Quickstart

### Q.1 Requirements

* C++11 compatible compiler
* Compiling examples: CMake 3.1+
* Compiling/running tests: CMake 3.11+ and Python 3.5+

GTest is downloaded, compiled and linked automatically.

### Q.2 Running examples

Steps to run examples:
* Clone repo: `git clone https://github.com/kongaskristjan/fire-hpp`
* Create build and change directory: `cd fire-hpp && mkdir build && cd build`
* Configure/build: `cmake .. && cmake --build .` (or substitute latter command with appropriate build system invocation, eg. `make -j8` or `ninja`)
* If errors are encountered, disable pedantic warnings and treating warnings as errors with `cmake -D DISABLE_PEDANTIC= ..` (though you are encouraged to open an issue) (you likely need to clear the build directory).
* Run: `./examples/basic --help` or `./examples/basic -x=3 -y=5` 

## T. Tutorial

Let's go through each part of the following example.

```
int fired_main(int x = fire::arg("x"), int y = fire::arg("y")) { // Define and convert arguments
    std::cout << x + y << std::endl; // Use x and y, they're ints
    return 0;
}

FIRE(fired_main) // call fired_main()
```

* __FIRE(function name)__
`FIRE(fired_main)` expands into the actual `main()` function that defines your program's entry point and fires off `fired_main()`. `fired_main` is called without arguments, thus compiler is forced to use the default `fire::arg` values.

* __fire::arg(identifier)__
 A constructor that accepts the name/shorthand/position of the argument. The library prepends a single dash to single-character shorthands and two dashes to multi-character names (eg. `-x` and `--longer-name`). `fire::arg` objects should be used as default values for fired function parameters. See [documentation](#fire_arg) for more options.

* __int fired_main(arguments)__
This is what you perceive as the program entry point. All arguments must be `bool`, integral, floating-point, `fire::optional<T>`, `std::string` or `std::vector<T>` type and default initialized with `fire::arg` objects (Failing to initialize properly results in undefined behavior!). See [conversions](#conversions) to learn how each of them affects the CLI.

## D. Documentation

### <a id="fire"></a> D.1 FIRE(...) and FIRE_NO_SPACE_ASSIGNMENT(...)

`FIRE(...)` and `FIRE_NO_SPACE_ASSIGNMENT(...)` both create the main function to parse arguments and call `...`, however, they differ in how arguments are parsed. `FIRE(...)` parses `program -x 1` as `program -x=1`, but `FIRE_NO_SPACE_ASSIGNMENT(...)` parses `-x` as a flag and `1` as a positional argument.

To use positional or vector arguments, `FIRE_NO_SPACE_ASSIGNMENT(...)` must be used. There are two reasons:

* Mixing positional and named arguments with space-separated values makes a bad CLI anyway, eg: `program a -x b c` doesn't seem like `-x=b` with `a` and `c` as positional.
* Implementing such a CLI within Fire API is likely impossible without using exceptions.
 
### D.2 <a id="fire_arg"></a> fire::arg(identifier[, description[, default_value]])

#### <a id="identifier"></a> D.2.1 Identifier

Identifier used to find arguments from command line. Can either be
* `const char * name`: named argument
    * Example: `int fired_main(int x = fire::arg("x"));`
    * CLI usage: `program -x=1`


* `{const char * shorthand, const char * full_name}`: named argument with a short-hand (single character) and long name
    * Example: `int fired_main(int x = fire::arg({"x", "long-name"}));`
    * CLI usage: `program -x=1`
    * CLI usage: `program --long-name=1`


* `int position`: positional argument (requires [no space assignment mode](#fire))
    * Example: `int fired_main(int x = fire::arg(0));`
    * CLI usage: `program 1`


* `{int position, const char * name}`: positional argument with name (name is only used for help) (requires [no space assignment mode](#fire))
    * Example: `int fired_main(int x = fire::arg({0, "name"}));`
    * CLI usage: `program 1`

#### <a id="description"></a> D.2.2 Descrpition (optional)

`std::string` argument description for `--help` message.

* Example: `int fired_main(int x = fire::arg("x", "an argument"));`
* CLI usage: `program --help`
* Output:
```
    Usage:
      ./examples/basic -x=<INTEGER>


    Options:
      -x=<INTEGER>  an argument
```

#### <a id="default"></a> D.2.3 Default value (optional)

Default value if no value is provided through command line. Can be either `std::string`, integral or floating-point type and `fire::arg` must be converted to that same type. This default is also displayed on the help page.

* Example: `int fired_main(int x = fire::arg("x", "", 0));`
* CLI usage: `program` -> `x==0`
* CLI usage: `program -x=1` -> `x==1`

### <a id="conversions"></a> D.3 fire::arg conversions

To conveniently obtain arguments with the right type and automatically check the validity of input, `fire::arg` class defines several implicit conversions.

#### <a id="standard"></a> D.3.1 std::string, integral, or floating point

Converts the argument value on command line to the respective type. Displays an error if the conversion is impossible or default value has wrong type.

* Example: `int fired_main(std::string name = fire::arg("name"));`
* CLI usage: `program --name=fire` -> `name=="fire"`
* Example: `int fired_main(double x = fire::arg("x"));`
* CLI usage: `program -x=blah` -> `Error: value blah is not a real number`

#### <a id="optional"></a> D.3.2 fire::optional

Used for optional arguments without a reasonable default value. This way the default value doesn't get printed in a help message. The underlying type can be `std::string`, integral or floating-point.

`fire::optional` is a tear-down version of [`std::optional`](https://en.cppreference.com/w/cpp/utility/optional), with compatible implementations for [`has_value()`](https://en.cppreference.com/w/cpp/utility/optional/operator_bool), [`value_or()`](https://en.cppreference.com/w/cpp/utility/optional/value_or) and [`value()`](https://en.cppreference.com/w/cpp/utility/optional/value).

* Example: `int fired_main(fire::optional<std::string> name = fire::arg("name"));`
* CLI usage: `program` -> `name.has_value()==false`, `name.value_or("default")=="default"`
* CLI usage: `program --name="fire"` -> `name.has_value()==true` and `name.value_or("default")==name.value()=="fire"`

#### <a id="flag"></a> D.3.3 bool: flag argument

Boolean flags are `true` when they exist on command line and `false` when they don't. Multiple single-character flags can be packed on command line by prefixing with a single hyphen: `-abc <=> -a -b -c`

* Example: `int fired_main(bool flag = fire::arg("flag"));`
* CLI usage: `program` -> `flag==false`
* CLI usage: `program --flag` -> `flag==true`

### <a id="vector"></a> D.4 fire::arg::vector([description])

A method for getting all positional arguments (requires [no space assignment mode](#fire)). The constructed object can be converted to `std::vector<std::string>`, `std::vector<integral type>` or `std::vector<floating-point type>`. Description can be supplied for help message. Using `fire::arg::vector` forbids extracting positional arguments with `fire::arg(index)`.

* Example: `int fired_main(vector<std::string> params = fire::arg::vector());`
* CLI usage: `program abc xyz` -> `params=={"abc", "xyz"}`
* CLI usage: `program` -> `params=={}`

## Development

This library uses extensive testing. Unit tests are located in `tests/`, while `examples/` are used as integration tests. The latter also ensures examples are up-to-date. Before committing, please verify `python3 ./build/tests/run_standard_tests.py` succeed.

v0.1 release is tested on:
* Arch Linux gcc==10.1.0, clang==10.0.0: C++11, C++14, C++17, C++20
* Ubuntu 18.04 clang=={3.5, 3.6, 3.7, 3.8, 3.9, 4.0}: C++11, C++14 and clang=={5.0, 6.0, 7.0, 8.0, 9.0}: C++11, C++14, C++17
* Ubuntu 18.04 gcc=={4.8, 4.9}: C++11 and gcc=={5.5, 6.5, 7.5, 8.4}: C++11, C++14, C++17
* Windows 10, MSVC=={19.26} (2019 Build Tools): C++11, C++14, C++17
* Mac OS, XCode=={11.5}: C++11, C++14, C++17

### TODO list:

#### Current status

* Automatic testing for error messages
* Improve help messages
    * Refactor `log_elem::type` from `std::string` -> `enum class`
    * Help messages with better organization (separate positional arguments, named arguments, flags, etc. in `Usage` and descriptions)
    * Program description
* Ensure API user gets an error message when using required positional arguments after optional positional arguments
* `save(...)` keyword enclosing `arg`, which will save the program from exiting even if not all required arguments are present or correct (eg. for `--version`)
* `program --flags  --  --interpret-everything-as-positional-arguments-after-double-dash`
* Remove exceptions

#### v0.2 release

* If exceptions are still enabled, allow positional arguments in both FIRE(...) and FIRE_NO_SPACE_ASSIGNMENT(...)
* Subcommands (with separate help messages for each subcommand)
* Allow specifying positional arguments as named arguments, if name exists
