
# Fire-hpp's algorithm

Let's start with the basics. Given a function definition

```c++
int fired_main(int x = fire::arg("-x"), int y = fire::arg("-y")) {
    std::cout << x + y << std::endl;
    return 0;
}
```

and a macro call `FIRE(fired_main)`, how does fire-hpp fill in integers `x` and `y`?

The `FIRE()` macro expands into something that parses arguments and calls `fired_main()`:

```c++
int main(int argc, char ** argv) {
    parse_arguments(argc, argv);
    fired_main();
    return 0;
}
```

The trick here is that `fired_main()` is called without arguments. Because of that, the compiler is forced to use the default parameters, which are `fire::arg()` objects. These objects are implicitly converted to the correct argument type. Notice that at this point, all information about the argument - it's type, name, etc. is known. Thus the correct value can be returned, based on the previously parsed and statically stored data by `parse_arguments()`.

If a value is impossible to parse, eg. in this case the user submitted a non-integer, fire-hpp prints an error and exits the program with the `exit()` function. This way the program is exited before control flow has a chance to execute `fired_main()` body.

## Help messages

What happens when the user executes `program --help`? During each conversion operation described above, information about the arguments is stored to static variables. During the last argument's conversion, Fire has all information about expected arguments. The help message is printed and the program is exited before `fired_main()` body executes. Note that the user possibly (probably) hasn't filled the required arguments, thus `fire::arg` conversions must return dummy data.

<a id="last_argument"></a> Here's a problem: how do we know when are we converting the last argument? The conversion itself presents no obvious indication. However, as can be seen from the documentation, correct (= sensible) API usage guarantees that, for every `fired_main()` function argument there is exactly one `fire::arg` object converted to it's respective type. Function arguments can be easily counted (thanks C++11) and stored to static variables before calling `fired_main()`. And `fire::arg` conversions can also be counted with static variables. The conversion which equalizes the counts is the last one.

## Parsing ambiguities - need for introspection

What has been described above almost works, and indeed, this was the algorithm of v0.1. However, a careful reader might notice that some cases are impossible to solve with the above described method. In what has been described above, the library must return a correct argument value it has even seen other arguments. In most cases, this might not be a problem, but some instances are inherently ambiguous. Let's see an example.

Let's consider the following two `fired_main()` functions

```c++
int fired_main1(int positional = fire::arg(0), int x = fire::arg("-x"), bool y = fire::arg("-y")) {
}

int fired_main2(int positional = fire::arg(0), bool x = fire::arg("-x"), int y = fire::arg("-y")) {
}
```

The two functions given are otherwise the same, except that flag arguments are `-y` in the first function and `-x` in the second.

So, let's say both programs are called like this: `program -x 0 -y 1`. If you carefully analyze the examples provided, you should conclude that arguments must be interpreted as
* `fired_main1`: `program -x {x's value} -y {positional}` (because `-y` is a flag)
* `fired_main2`: `program -x {positional} -y {y's value}` (because `-x` is a flag)

In one case, positional's value is 1, in another, it's 0.

Now, let's say that our compiler chooses `positional` argument to be converted first (compilers actually have total freedom over that). In this case, `fire` has no information whether it is the first or second case. There is no way for the library to determine at this point whether 0 or 1 should be returned.

In v0.1, this is solved by forcing the user to specify argument values with an equals sign (eg. `program -x=0`), as this solves the ambiguity. However, this is a serious limitation with a significant usability cost (especially as some terminals don't autocomplete paths this way).

### Function signatures don't help either

You might think that argument information could be extracted from the function signature, but this unfortunately isn't the case. Function signatures don't contain default values, so for example the two ambiguous cases have respectively argument signatures `(int, int, bool)` and `(int, bool, int)`. These are essentially the same, as argument order is irrelevant anyway.

### v0.2+ Introspection using exceptions 

There is, however, a trick to solve the problem above. It is actually possible to introspect `fired_main()` arguments by first calling the function, but then throwing an exception while converting the last argument ([we know when we are converting the last argument](#last_argument)). All the arguments are logged to static variables, and then a second `fired_main()` (the "real" one) invocation is done. Thus `FIRE()` macro could expand into something like this:

```c++
int main(int argc, char ** argv) {
    ... // Store the number of arguments in fired_main(), so we know when to throw
    try {
        fired_main(); // This always throws during last argument conversion. fired_main() body is not executed, but argument info is stored
    } catch (fire::_escape_exception) { // Oops, nothing to do here
    }
    
    parse_arguments(argc, argv); // Now we have our arguments and we can unambigously parse the input 
    fired_main(); // The actual fired_main() invocation with correct conversion values, don't throw this time
    return 0;
}
```
