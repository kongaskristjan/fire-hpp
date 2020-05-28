
#include <iostream>
#include <vector>
#include <string>
#include "../fire.hpp"

using namespace std;

// Program inspired by `ls`. Takes a variable number of strings, and prints them.
// Optionally sorts them or prints them line by line.

int fired_main(vector<string> strings = fire::arg::vector("strings to be printed"),
               bool lines = fire::arg({"o", "one-per-line"}),
               bool do_sort = fire::arg({"s", "sort"})) {
    if(do_sort)
        std::sort(strings.begin(), strings.end());

    for(size_t i = 0; i < strings.size(); ++i) {
        cout << strings[i];
        if(lines || i == strings.size() - 1)
            cout << endl;
        else
            cout << " ";
    }

    return 0;
}

FIRE_POSITIONAL(fired_main)
