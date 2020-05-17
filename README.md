# Fire for C++

## Development

This library uses extensive testing. Unit tests are located in Tests.cpp, while examples are used as integration tests. Those tests are necessary, as CLI cannot be easily verified in the unit testing framework. This also ensures that examples are always up-to-date and work as intended. Before committing, please ensure `./build/run_tests` and `./build/run_examples.py` succeed.

### Todo

* Parameter descriptions
* Gathering arguments
    * Help generation
* Multiple keys for arguments
* Unnamed arguments
    * Positionals (`positional`, `named` -> `arg`)
    * Vectors
* High quality parsing
    * Boolean parameters (with `"-xy" == "-x -y"` support)
    * `"-x 1" == "-x=1"`
    * Support "hyphens" for strings
* Add more types
    * `unsigned`
    * Short/long versions of numeric types, eg. `short`, `long`, `long long`
* Linking together multiple compilation units
* Deferring command line user issues to always show argument parsing errors
* Testing for error messages
* Modules

### Design dreams

These goals are likely impossible, so I call them dreams.

#### Current interface

```
int fire_main(int x = named("x", 0), int y = named("y", 0)) {
}

FIRE(fire_main)
```

#### FIRE_MAIN() interface

Likely impossible. Also, where are you going to supply program description?

```
int FIRE_MAIN(int x = named("x", 0), int y = named("y", 0)) {
}
```

#### FIRE_MAIN() + optimal parameter syntax

Likely impossible. Up to my knowledge you can't extract `x` and `y` names (necessary for parsing command line). Also, what happens when you want to supply parameter descriptions?

```
int FIRE_MAIN(int x = 0, int y = 0) {
}
```
