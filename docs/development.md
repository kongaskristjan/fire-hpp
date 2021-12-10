
## Development

This library uses extensive testing. Unit tests are located in `tests/`, while `examples/` are used as integration tests. The latter also ensures examples are up-to-date. Before committing, please verify `python3 ./build/tests/run_standard_tests.py` succeed. Releases are also tested on many platforms with `python3 ./tests/run_release_tests.py`.

v0.2 release is tested on:
* Arch Linux: gcc==10.2.0, clang==10.0.1: C++11, C++14, C++17, C++20
* Ubuntu 18.04: gcc=={4.8, 4.9}: C++11 and gcc=={5.5, 6.5, 7.5, 8.4}: C++11, C++14, C++17
* Ubuntu 18.04: clang=={3.5, 3.6, 3.7, 3.8, 3.9, 4.0}: C++11, C++14 and clang=={5.0, 6.0, 7.0, 8.0, 9.0}: C++11, C++14, C++17
* Windows 10: MSVC=={19.26} (2019 Build Tools): C++11, C++14, C++17
* Mac OS: XCode=={12.0} (requires CMake 3.18.1+): C++11, C++14, C++17

### Roadmap:

#### v0.3 release

* Sufficient information for contributing
    * Create a document describing the internals of this project
    * Document main parts of the code

#### v0.4 release

* Solve Windows non-ascii character input
* Subcommands (eg. `git add` and `git show`, which may have different flags/options)
* Parameter structs (eg. get all arguments as one struct with fire::arg initializations)

#### v1.0 release

* Ensure API user gets an error message when using required positional arguments after optional positional arguments
* Allow converting named `fire::arg` to `std::vector` or `std::map` (allows multiple occurrences of an argument, like cmake's `-D`)
