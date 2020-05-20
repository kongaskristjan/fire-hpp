# Fire for C++

## Development

This library uses extensive testing. Unit tests are located in Tests.cpp, while examples are used as integration tests. Those tests are necessary, as CLI cannot be easily verified in the unit testing framework. This also ensures that examples are always up-to-date and work as intended. Before committing, please ensure `./build/run_tests` and `./build/run_examples.py` succeed.

### Todo

* High quality parsing
    * Boolean parameters (with `"-xy" == "-x -y"` support)
    * `"-x 1" == "-x=1"`
* Unnamed arguments
    * Positional parsing mode
    * Positionals
    * Vectors
* Linking together multiple compilation units
* Add more types
    * `unsigned`
    * Short/long versions of numeric types, eg. `short`, `long`, `long * Testing for error messages
* Documentation
* Test on Windows, Mac

#### v0.1 release

* super/super_arg keyword for argument, which will save program from exiting even if not all required arguments are present or correct (eg. for --version) (don't immediately finish parsing if there are user made errors)
* Remove exceptions
* Modules (with separate help messages for each module (otherwise impossible without exceptions))

### Design ~~goals~~ dreams

These goals are likely impossible.

#### Current interface

```
int fire_main(int x = arg("x", 0), int y = arg("y", 0)) {
}

FIRE(fire_main)
```

#### FIRE_MAIN() interface

Likely impossible. Also, where are you going to supply program description?

```
int FIRE_MAIN(int x = arg("x", 0), int y = arg("y", 0)) {
}
```

#### FIRE_MAIN() + optimal parameter syntax

Likely impossible. Up to my knowledge you can't extract `x` and `y` names (necessary for parsing command line). Also, what happens when you want to supply parameter descriptions?

```
int FIRE_MAIN(int x = 0, int y = 0) {
}
```
