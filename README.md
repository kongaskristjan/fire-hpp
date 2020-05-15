# Fire for C++

## Development

This library uses extensive testing. Unit tests are located in Tests.cpp, while examples are used as integration tests. Those tests are necessary, as CLI cannot be easily verified in the unit testing framework. This also ensures that examples are always up-to-date and work as intended. Before committing, please ensure `./build/run_tests` and `./build/run_examples.py` succeed.

### Todo

* Gathering arguments
    * Error when providing arguments that don't exist
    * Help generation
* Multiple keys for arguments
* High quality parsing
    * Boolean parameters (with `"-xy" == "-x -y"` support)
    * Positional arguments
    * Support "hyphens" for strings
    * Error handling
* Optional arguments with C++11 compatibility
* Modules
* FIRE_MAIN() (see design goals) (is it even possible?)
* Optimal parameter syntax (see design goals) (likely impossible)

### Design goals

#### Current interface

```
int fire_main(int x = Named("x", 0), int y = Named("y", 0)) {
}

FIRE(fire_main)
```

#### FIRE_MAIN() interface: Complicated, but likely possible

```
int FIRE_MAIN(int x = Named("x", 0), int y = Named("y", 0)) {
}
```

#### FIRE_MAIN() + optimal parameter syntax

Likely impossible, up to my knowledge you can't extract `x` and `y` names (necessary for parsing command line). Also, what happens when you want to supply parameter descriptions? 

```
int FIRE_MAIN(int x = 0, int y = 0) {
}
```
