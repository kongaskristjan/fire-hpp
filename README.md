
## Why Fire for C++?

Fire for C++ is a super-clean library for creating command line interfaces. Here's a complete example of adding two numbers with a CLI:
 ```
#include <iostream>
#include <fire.hpp>

int fire_main(int x = fire::arg("x"), int y = fire::arg("y")) { // Define, parse and convert arguments
    std::cout << x + y << std::endl;  // Use x and y, they're ints.
    return 0;
}

FIRE(fire_main)
```

Usage:
```
$ ./add -x=1 -y=2
3
```

Also,
* a meaningful help message is generated if prompted with `--help`, stating required arguments and their types.
* an error message is displayed for incorrect usage.

In addition to what was shown above, this library supports
* integer, floating-point and string parameters, with automatic conversions
* optional parameters
* ~~positional parameters and unlimited number of parameters (through std::vector)~~
* flags
* parameter descriptions
* typical constructs, such as expanding `-abc <=> -a -b -c` and `-x=1 <=> -x 1`

In addition, this library
* comes with thorough testing
* ~~is cross-platform, works with Linux, Windows and Mac OS~~
* is single header
* ~~comes under Boost licence, meaning you can copy `fire.hpp` to any project without further disclaimers, no strings attached. Simply don't touch the licence.~~ 



## Development

This library uses extensive testing. Unit tests are located in Tests.cpp, while `examples/` are used as integration tests. These integration tests also ensure that examples are always up-to-date and work as intended. Before committing, please verify `./build/run_tests` and `./build/run_examples.py` succeed.

### Todo

* Unnamed arguments
    * Positional parsing mode
    * Positionals
    * Vectors
* Linking together multiple compilation units
* Add more types
    * `unsigned`
    * Short/long versions of numeric types, eg. `short`, `long`, `long long`
* Documentation
* Systematic, tested examples
* Test on Windows, Mac
* Automatic testing for error messages

#### v0.1 release

* super/super_arg keyword for argument, which will save program from exiting even if not all required arguments are present or correct (eg. for --version) (don't immediately finish parsing if there are user made errors)
* Remove exceptions
* Support for wide character strings

#### v0.2 release

* Modules (with separate help messages for each module (otherwise impossible without exceptions))
