
# Development

The algorithm for introspecting and filling in arguments is rather complex, so I strongly suggest reading [this document](https://github.com/kongaskristjan/fire-hpp/blob/master/docs/algorithm.md) before trying to understand the included library file.

## Testing

This library uses extensive testing. Unit tests are located in `tests/`, while `examples/` are used as integration tests. The latter also ensures examples are up-to-date. Before committing, please verify `python3 ./build/tests/run_standard_tests.py` succeed. Releases are also tested on many platforms with `python3 ./tests/run_release_tests.py`.

v0.3 release is tested on:
* Arch Linux: gcc==11.1.0, clang==13.0.0: C++11, C++14, C++17, C++20
* Ubuntu 18.04: gcc=={5.5, 6.5, 7.5, 8.4}: C++11, C++14, C++17
* Ubuntu 18.04: clang=={3.5, 3.6, 3.7, 3.8, 3.9, 4.0}: C++11, C++14 and clang=={5.0, 6.0, 7.0, 8.0, 9.0}: C++11, C++14, C++17
* Ubuntu 20.04: gcc=={9.3, 10.3} and clang=={10.0, 11.0, 12.0}: C++11, C++14, C++17, C++20
* Windows 10: MSVC=={22.??} (2022 Build Tools): C++11, C++14, C++17, C++20
* Mac OS: XCode AppleClang=={13.0.0}: C++11, C++14, C++17

## Roadmap:

---

### v0.3 release

Almost ready!

---

### v0.4 release

#### Solve Windows non-ascii character input (+ perhaps add conversion to wstrings)

Non-ascii characters aren't handled in Windows right now. The solution is to use ifdefs to determine whether to use `wmain()` or `main()`, and convert wide strings to utf-8 strings. Optional: also add wstring as a valid conversion type, though [its usage should probably be discouraged](http://utf8everywhere.org/).

Note that this would be a more portable solution than most libraries can have, as usually the library has no control on whether to use main or wmain.

#### Subcommands (eg. `git add` and `git show`, which may have different flags/options)

Something like that:
```c++
int add(int x = fire::arg("x"), int y = fire::arg("y")) { std::cout << x + y << std::endl; return 0; }
int sub(int x = fire::arg("x"), int y = fire::arg("y")) { std::cout << x - y << std::endl; return 0; }

FIRE_SUBCOMMAND(add, "add", "Add two numbers")
FIRE_SUBCOMMAND(sub, "subtract", "Subtract two numbers")

// Option 1: no default command
FIRE()

// Option 2: with default
int fired_main(std::string text = fire::arg("text")) { std::cout << text << std::endl; return 0; }
FIRE(fired_main)
```

Positional arguments should be disallowed for `fired_main`, as these interfere with subcommands.

Implementation: The `FIRE_SUBCOMMAND` can be registered like presented [here](https://artificial-mind.net/blog/2020/10/17/static-registration-macro]). If subcommands are present, then the first step is to check whether one is referenced, remove the command from arguments, and proceed to the function as normal.

#### Parameter structs (eg. get all arguments as one struct with fire::arg initializations)

```c++

struct A: public fire::structure {
    bool flag = fire::arg("--flag");
    std::string value = fire::arg("--value");
};

struct B: public fire::structure {
    // Perhaps some arguments here too
};

int fired_main(A a = fire::object(), B b = fire::object()) {
    if(a.flag)
        std::cout << a.value << std::endl;
    return 0;
}

FIRE(fired_main)
```

Structures can be nested (eg. in struct `A`, you could write `C c = fire::object();`, pointing to another struct).

Parameter structs are useful for
* passing many parameters to other functions
* combining with subcommands, eg. if two subcommands have a similar set of parameters
* avoiding too many arguments in fired_main

Implementation: Counting arguments should be stopped during struct conversion. Other than that I don't see much work here.

---

### v1.0 release

#### C++17 only: `fire::arg` conversion to `std::filesystem::path` + exists()/directory() and other optional checks
#### `fire::arg` conversion to `std::vector` (allows multiple occurrences of an argument)
