#include <iostream>
#include "../fire.hpp"

using namespace std;

int fired_main(bool flag_a = fire::arg({"a", "flag-a"}), bool flag_b = fire::arg({"b", "flag-b"})) {
    cout << flag_a << " " << flag_b << endl;
    return 0;
}

FIRE(fired_main)
