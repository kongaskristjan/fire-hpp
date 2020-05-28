#include <iostream>
#include "../fire.hpp"

using namespace std;
using namespace fire;

int fired_main(int x = arg({"x", "x-coord"}, "", 0), int y = arg({"y", "y-coord"}, "", 0)) {
    cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fired_main)
