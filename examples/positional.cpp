#include <iostream>
#include "../fire.hpp"

using namespace std;
using namespace fire;

int fire_main(int x0 = arg(0), fire::optional<int> x1 = arg(1)) {
    std::cout << x0 << " " << x1.value_or(0) << endl;
    return 0;
}

FIRE_POSITIONAL(fire_main)
