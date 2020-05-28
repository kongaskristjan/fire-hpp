#include <iostream>
#include "../fire.hpp"

using namespace std;
using namespace fire;

int fired_main(int x = arg("x"), int y = arg("y")) {
    cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fired_main)
