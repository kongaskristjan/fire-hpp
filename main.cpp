#include <iostream>
#include "fire.h"

using namespace std;
using namespace fire;

int fire_main(
        int i = Named("-i", 0),
        double f = Named("-f", 0),
        string s = Named("-s", "empty")) {
    cout << "i: " << i << "    f: " << f << "    s: " << s << endl;
    return 0;
}

FIRE(fire_main)
