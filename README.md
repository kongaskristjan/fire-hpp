
# Fire for C++

Fire for C++ is a library for generating command line interfaces from function signatures. Here's a complete example of adding two numbers with a CLI:
 ```
#include <iostream>
#include <fire.hpp>

int fire_main(int x = fire::arg("x"), int y = fire::arg("y")) {
    std::cout << x + y << std::endl;
    return 0;
}

FIRE(fire_main)
```

That's it. Usage is as follows:

```
$ ./add -x=1 -y=2
3
```

As you likely expect,
* a meaningful help message is generated if prompted with `--help`, stating required arguments and their types.
* an error message is displayed for incorrect usage.

## What's covered?

All the standard stuff, like
* flags
* named and positional parameters
* required and optional parameters
* conversions to integer, floating-point, `std::string` and `std::vector`
* parameter descriptions
* typical constructs, such as expanding `-abc <=> -a -b -c` and `-x=1 <=> -x 1`

In addition, this library
* comes with thorough testing
* ~~works with Linux, Windows and Mac OS~~
* is a single header
* ~~comes under very permissive Boost licence~~

## What's not covered yet?

I've mostly been doing dealing with adding all the functionality and ensuring it's reliability, but skimped on user messages. Most pressing issues:

* I currently only test whether error messages are displayed at all, but not the exact message. Because of that, the messages are sometimes misleading or don't contain enough information to solve the problem.
* Help message could separate different classes of parameters. Also, the way positional arguments are displayed is clearly unoptimal.
* No program description.
* `program_name --version` will make the fire module complain and immediately exit, if there's a required argument. This way there's no chance to show version info. Note however that `program_name --help` works, so it's not too complicated to port the same logic to user defined flags.

Understandably, these are big issues, given that we are talking about a CLI library. However, none of them seem like road-blockers and if everything goes well, they are addressed in v0.2 (see [TODO](#todo-list)).

Please don't yet report bugs in error messages, as I am aware of them.



## Development

This library uses extensive testing. Unit tests are located in `tests/`, while `examples/` are used as integration tests. These integration tests also ensure that examples are always up-to-date and work as intended. Before committing, please verify `./build/tests/run_all_tests.py` succeed.

### TODO list:

#### Current state

* Documentation
* Systematic, tested examples
* Licence fire.hpp and tests under Boost, examples under Unlicence
* Test on Windows, Mac

#### v0.1 release

* Better error messages
    * Automatic testing for error messages
    * Improve CLI user errors
    * Improve API user errors
* Improve help messages
    * Refactor `log_elem::type` from `std::string` -> `enum class`
    * Help messages with better organization (separate positional arguments, named arguments, flags, etc. in `Usage` and descriptions)
    * Program description
* `super` keyword for `arg`, which will save program from exiting even if not all required arguments are present or correct (eg. for `--version`)
* Remove exceptions

#### v0.2 release

* Support for wide character strings
* Modules (with separate help messages for each module (otherwise impossible without exceptions))

## Acknowledgements

* Fire for C++ is heavily inspired by [python-fire](https://github.com/google/python-fire)
