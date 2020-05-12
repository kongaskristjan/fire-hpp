#include <iostream>
#include "fire.h"

int fire_main(int x = fire::Named("x", 0), int y = fire::Named("y", 0)) {
    std::cout << x << " + " << y << " = " << x + y << std::endl;
    return 0;
}

FIRE(fire_main)
