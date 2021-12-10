
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

### What's covered?

* [flags](#flag); [named and positional](#identifier) parameters; [variadic parameters](#variadic)
* [optional parameters](#optional)/[default values](#default)
* conversions to [integer, floating-point and `std::string`](#standard) with automatic error checking
* [program](#fire)/[parameter](#identifier) descriptions
* standard constructs, such as `-abc <=> -a -b -c` and `-x=1 <=> -x 1`

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
* Run: `./examples/add --help` or `./examples/add -x=3 -y=5`

### Q.3 Usage with cmake

* [See here](https://github.com/kongaskristjan/fire-hpp/tree/master/docs/cmake.md)

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
This is what you perceive as the program entry point. All arguments must be `bool`, integral, floating-point, `fire::optional<T>`, `std::string` or `std::vector<T>` type and default initialized with `fire::arg` objects (failing to initialize properly results in undefined behavior!). See [conversions](#conversions) to learn how each of them affects the CLI.

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

#### <a id="constraints"></a> D.2.3 Constraints

Constraints can be applied to arguments by calling `fire:arg`'s constraint methods:
* `fire::arg().min(T minimum)` - specifies minimum value
* `fire::arg().max(T maximum)` - specifies maximum value
* `fire::arg().bounds(T minimum, T maximum)` - specifies both minimum and maximum values
* `fire::arg().one_of({...})` - specifies possible values

These methods
1) check whether user supplied value fits the constraint and emit a proper error message if condition is not met
2) append the constraint to argument's help message

* Example: `int fired_main(int x = fire::arg("-x").bounds(-1000, 1000));`
  * CLI usage: `program -x=100` -> `x==100`
  * CLI usage: `program -x=-10000` -> `Error: argument -x value -10000 must be at least -1000`
  * CLI usage: `program --help` -> `-x=INTEGER  description [-1000 <= x <= 1000]` (one of the lines)

* Example: `int fired_main(std::string s = fire::arg("-s").one_of({"hi", "there"}));`
  * CLI usage: `program -s=hi` -> `s==hi`
  * CLI usage: `program -s=hello` -> `Error: argument -s value must be one of (hi, there), but given was 'hello'`

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

For an optional argument with a sensible default value, see [default value](#default).

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

### <a id="post_functions"></a> D.4 Post fired_main() functions

#### <a id=""></a> D.4.1.1 Print help or error message with fire formatting

* `fire::print_help()` - print the help message
* `fire::input_error(const string &msg)` - print error message and exit program
* `fire::input_assert(bool pass, const std::string &msg)` - if `pass` is not satisfied, print error message and exit program

#### <a id=""></a> D.4.1.2 Helper function for getting named argument names in assert messages:

`std::string fire::helpful_name(const string &name)` - return user called name of the specified argument (given by one name) if it exists, otherwise return empty string.

* Example: `int fired_main(optional<int> value = fire::arg({"-v", "--value"}));`
  * CLI usage: `program` -> `fire::helpful_name("--value") == ""`
  * CLI usage: `program -v=2` -> `fire::helpful_name("--value") == "-v"`
  * CLI usage: `program --value=2` -> `fire::helpful_name("--value") == "--value"`
* Typical usage: `fire::input_assert(value > 0, "Argument " + fire::helpful_name("--value") + " must be greater than 0")`

`std::string fire::helpful_name(int pos)` - return correctly formatted positional argument number

#### <a id="raw_args"></a> D.4.2 Accessing raw arguments

Some third party libraries require access to raw argc/argv. This is gained through `fire::raw_args` (of type `fire::c_args`), which has `argc()` and `argv()` methods for accessing the arguments.

Examples:

* Usage without modification:
```
int argc = fire::raw_args.argc();
char ** argv = fire::raw_args.argv();
non_modifying_call(argc, argv);
```

* Usage with modification:
```
fire::c_args raw_args_copy = fire::raw_args; 
int& argc = raw_args_copy.argc();
char ** argv = raw_args_copy.argv();
modifying_call(argc, argv);
// Once out of scope, raw_args_copy releases argv strings
```

You also need [`FIRE_ALLOW_UNUSED(...)`](#fire) if the third party library processes it's own arguments.

## L. Links

* [CMake usage](https://github.com/kongaskristjan/fire-hpp/blob/master/docs/cmake.md)
* [Conan usage](https://github.com/kongaskristjan/fire-hpp/blob/master/docs/conan.md)
* [Development](https://github.com/kongaskristjan/fire-hpp/blob/master/docs/development.md)
