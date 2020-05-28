
#include <iostream>
#include "../fire.hpp"

using namespace std;

int fired_main(
        int i = fire::arg("i", "Integer"),
        double r = fire::arg("r", "Real number"),
        string s = fire::arg("s", "String"),
        int def_i = fire::arg("def-i", "Default value integer", 1),
        double def_r = fire::arg("def-r", "Default value real number", 1.0),
        string def_s = fire::arg("def-s", "Default value string", "default text"),
        fire::optional<int> opt_i = fire::arg("opt-i", "Optional integer"),
        fire::optional<double> opt_r = fire::arg("opt-r", "Optional real number"),
        fire::optional<string> opt_s = fire::arg("opt-s", "Optional string"),
        unsigned pos_i = fire::arg(0),
        long double pos_r = fire::arg(1),
        std::string pos_s = fire::arg(2),
        fire::optional<int> opt_pos_i = fire::arg(3),
        fire::optional<double> opt_pos_r = fire::arg(4),
        fire::optional<std::string> opt_pos_s = fire::arg(5)
        ) {
    return 0;
}

FIRE(fired_main)
