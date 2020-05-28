
#include <iostream>
#include "../fire.hpp"

using namespace std;

int fired_main(fire::optional<int> opt = fire::arg("optional"), int def = fire::arg("default", "", 0)) {
    cout << (opt.has_value() ? "true" : "false") << " " << (def != 0 ? "true" : "false") << endl;
    return 0;
}

FIRE(fired_main)