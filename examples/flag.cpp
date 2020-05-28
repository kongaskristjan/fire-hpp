#include <iostream>
#include "../fire.hpp"

using namespace std;
using namespace fire;

int fired_main(bool flag_a = arg({"a", "flag-a"}), bool flag_b = arg({"b", "flag-b"})) {
    cout << flag_a << " " << flag_b << endl;
    return 0;
}

FIRE(fired_main)
