
# Fire for C++

Fire for C++ is a library for creating a command line interface directly from function signature, inspired by [python-fire](https://github.com/google/python-fire). Here's a complete example of adding two numbers with a CLI:
 ```
#include <iostream>
#include <fire.hpp>

int fired_main(int x = fire::arg("x"), int y = fire::arg("y")) {
    std::cout << x + y << std::endl;
    return 0;
}

FIRE(fired_main)
```

That's it. And usage:

```
$ ./add -x=1 -y=2
3
```

As you likely expect,
* a meaningful help message is generated if prompted with `--help`, stating required arguments and their types.
* an error message is displayed for incorrect usage.

### What's covered?

All the standard stuff, like
* flags
* named and positional parameters
* required and optional parameters
* variable number of parameters (through `std::vector`)
* automatic conversions to integer, floating-point and `std::string`, with proper error reporting
* parameter descriptions
* typical constructs, such as expanding `-abc <=> -a -b -c` and `-x=1 <=> -x 1`

In addition, this library
* ~~works with Linux, Windows and Mac OS~~
* is a single header
* comes under very permissive [Boost licence](https://choosealicense.com/licenses/bsl-1.0/) (examples with [0-clause BSD](https://choosealicense.com/licenses/0bsd/))

### Requirements

* C++11 compatible compiler

Additionally, for compiling examples

* CMake 3.1+

Additionally, for compiling tests

* CMake 3.14+
* Python 3.5+

GTest gets downloaded, compiled and linked automatically.

## Q. Quick start

Let's go through each part of the above example.

```
int fired_main(int x = fire::arg("x"), int y = fire::arg("y")) { // Define and convert arguments
    std::cout << x + y << std::endl;
    return 0;
}

FIRE(fired_main) // call fired_main()
```

* <a id="quickfire"></a> __FIRE(...)__
`FIRE(fired_main)` expands into the actual `main()` function that defines your program's entry point and calls (fires off) `fired_main()`. `fired_main` is called without arguments, thus compiler is forced to use the default `fire::arg` values.

* __fire::arg(identifier)__
 A constructor that accepts the name/shorthand/position of the argument. The library prepends a single dash to single-character shorthands and two dashes to multi-character names: `-x` and `--longer-name`. `fire::arg` objects should be used as default values for fired function parameters and implicitly converted to arithmetic or `std::string` type. An error is reported when conversion is impossible.

* __int fired_main(...)__
This is your perceived program entry point. All arguments must be arithmetic or `std::string` type and default initialized with `fire::arg` objects (Failing to do so results in undefined behaviour!).

## D. Documentation

### <a id="fire_positional"></a> D.1 FIRE(...) and FIRE_NO_SPACE_ASSIGNMENT(...)

See `FIRE(...)` [quick start](#quickfire).

`FIRE(...)` and `FIRE_NO_SPACE_ASSIGNMENT(...)` both create a main function to parse arguments and call `...`, however they differ in how arguments are parsed. `FIRE(...)` parses `program -x 1` as `program -x=1`, but `FIRE_NO_SPACE_ASSIGNMENT(...)` parses `-x` as a flag and `1` as a separate positional argument. Thus in order to use valued named arguments in positional mode, `-x=1` need to be used. There a two reasons:

* Mixing positional and named arguments with space separated values makes a bad CLI anyway, eg: `program a -x b c` doesn't seem like `-x=b` with `a` and `c` as positional.
* Implementing such CLI is likely impossible due to the Fire API.

### D.2 fire::arg(identifier[, description[, default_value]])

#### D.2.1 Identifier

Identifier used to find arguments from command line. Can either be
* `const char *`: named argument
    * Example: `int fired_main(int x = fire::arg("x"));`
    * CLI usage: `program -x=1`


* `initializer_list<const char *>`: named argument with a short-hand (single character) and long name
    * Example: `int fired_main(int x = fire::arg({"x", "long-name"}));`
    * CLI usage: `program -x=1`
    * CLI usage: `program --long-name=1`


* `int`: positional argument (requires [positional mode](#fire_positional))
    * Example: `int fired_main(int x = fire::arg(0));`
    * CLI usage: `program 1`

#### D.2.2 Descrpition (optional)

`std::string` that gets printed when the program is prompted with `--help`.

* Example: `int fired_main(int x = fire::arg("x", "an argument"));`
* CLI usage: `program --help`
* Output:
```
    Usage:
      ./examples/basic -x=<INTEGER>


    Options:
      -x=<INTEGER>  an argument
```

#### D.2.3 Default value (optional)

Default value if no value is provided through command line. `std::string`, integral or floating point type. This default is also displayed in help page.

* Example: `int fired_main(int x = fire::arg("x", "", 0));`
* CLI usage: `program` -> `x=0`
* CLI usage: `program -x=1` -> `x=1`

### D.3 fire::arg conversions

In order to conveniently obtain parsed arguments and automatically check the validity of input, `fire::arg` class defines several implicit conversions.

#### D.3.1 std::string, integral, or floating point

Converts the argument value on command line to respective type. Displayes an error if conversion is impossible or default value is of wrong type.

* Example: `int fired_main(std::string name = fire::arg("name"));`
* CLI usage: `program --name=fire` -> `name="fire"`

#### D.3.2 fire::optional

Used for optional arguments without a reasonable default value. This way the default value doesn't get printed in a help message. The underlying type can be `std::string`, integral or floating point.

`fire::optional` is a tear-down version of [`std::optional`](https://en.cppreference.com/w/cpp/utility/optional), with compatible implementations for [`has_value()`](https://en.cppreference.com/w/cpp/utility/optional/operator_bool), [`value_or()`](https://en.cppreference.com/w/cpp/utility/optional/value_or) and [`value()`](https://en.cppreference.com/w/cpp/utility/optional/value).

* Example: `int fired_main(fire::optional<std::string> name = fire::arg("name"));`
* CLI usage: `program` -> `name.has_value()=false`
* CLI usage: `program --name="fire"` -> `name.has_value()=true` and `name.value()="fire"`

#### D.3.3 bool: flag argument

Boolean flags are `true` when they exist on command line and `false` when they don't. Multiple single-character flags can be packed on command line by prefixing with a single hyphen: `-abc <=> -a -b -c`

* Example: `int fired_main(bool flag = fire::arg("flag"));`
* CLI usage: `program` -> `flag=false`
* CLI usage: `program --flag` -> `flag=true`

### D.4 fire::arg::vector([description])

A method for getting all positional arguments (requires [positional mode](#fire_positional)). The constructed object can be converted to `std::vector<std::string>`, `std::vector<integral type>` or `std::vector<floating point type>`. Description can be supplied for help message. Using `fire::arg::vector` forbids extracting positional arguments with `fire::arg(index)`.

* Example: `int fired_main(vector<std::string> params = fire::arg::vector());`
* CLI usage: `program abc xyz` -> `params={"abc", "xyz"}`
* CLI usage: `program` -> `params={}`

## Development

This library uses extensive testing. Unit tests are located in `tests/`, while `examples/` are used as integration tests. The latter also ensure examples are up-to-date. Before committing, please verify `python3 ./build/tests/run_standard_tests.py` succeed. Releases must be tested on as many platforms as possible. The recommended way is by batch-testing with `python3 ../tests/run_release_tests.py ..` from `build/`. See it's `--help` page for more info.

v0.1 release tested on:
* Arch Linux gcc==10.1.0, clang==10.0.0: C++11, C++14, C++17 and C++20
* Ubuntu 18.04 clang=={3.5, 3.6, 3.7, 3.8, 3.9}: C++11, C++14 and clang=={4.0, 5.0, 6.0, 7.0, 8.0, 9.0}: C++11, C++14 and C++17
* Ubuntu 18.04 gcc=={4.8, 4.9}: C++11 and gcc=={5.5, 6.5, 7.5, 8.4}: C++11, C++14 and C++17

### TODO list:

#### Current state

* Improve CLI user errors
* Improve API user errors
* Test on Windows, Mac

#### v0.1 release

* Automatic testing for error messages
* Improve help messages
    * Refactor `log_elem::type` from `std::string` -> `enum class`
    * Help messages with better organization (separate positional arguments, named arguments, flags, etc. in `Usage` and descriptions)
    * Program description
* Ensure API user gets error message when using required positional arguments after optional positional arguments
* `save(...)` keyword enclosing `arg`, which will save program from exiting even if not all required arguments are present or correct (eg. for `--version`)
* Remove exceptions
* If exceptions are still enabled, allow positional arguments in both FIRE(...) and FIRE_NO_SPACE_ASSIGNMENT(...)

#### v0.2 release

* Modules (with separate help messages for each module (otherwise impossible without exceptions))
