#include <iostream>
#include "../fire.hpp"

using namespace std;

int fired_main(int x = fire::arg("x"), int y = fire::arg("y")) {
    cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fired_main)
