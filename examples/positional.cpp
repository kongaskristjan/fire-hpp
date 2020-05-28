#include <iostream>
#include "../fire.hpp"

using namespace std;

int fired_main(int x0 = fire::arg(0), fire::optional<int> x1 = fire::arg(1)) {
    std::cout << x0 << " " << x1.value_or(0) << endl;
    return 0;
}

FIRE_POSITIONAL(fired_main)
