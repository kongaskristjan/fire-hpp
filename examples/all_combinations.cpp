
#include <iostream>
#include "../fire.h"

using namespace std;

int fire_main(
        int i = fire::named("i", "Integer"),
        double r = fire::named("r", "Real number"),
        string s = fire::named("s", "String"),
        int def_i = fire::named("def-i", "Default value integer", 1),
        double def_r = fire::named("def-r", "Default value real number", 1.0),
        string def_s = fire::named("def-s", "Default value string", "default text"),
        fire::optional<int> opt_i = fire::named("opt-i", "Optional integer"),
        fire::optional<double> opt_r = fire::named("opt-r", "Optional real number"),
        fire::optional<string> opt_s = fire::named("opt-s", "Optional string")) {
    return 0;
}

FIRE(fire_main)
