#include <iostream>
#include "../fire.h"

using namespace std;
using namespace fire;

int fire_main(int x = Named("-x", 0), int y = Named("-y", 0)) {
    cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fire_main)
