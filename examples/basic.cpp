#include <iostream>
#include "../fire.hpp"

using namespace std;
using namespace fire;

int fire_main(int x = named("x", "", 0), int y = named("y", "", 0)) {
    cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fire_main)
