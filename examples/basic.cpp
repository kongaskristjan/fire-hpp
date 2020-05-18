#include <iostream>
#include "../fire.hpp"

using namespace std;
using namespace fire;

int fire_main(int x = arg("x", "", 0), int y = arg("y", "", 0)) {
    cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fire_main)
