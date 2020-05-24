
#include <iostream>
#include <vector>
#include <string>
#include "../fire.hpp"

using namespace std;

int fire_main(vector<string> files = fire::arg::vector("files to be printed"),
        bool lines = fire::arg({"1", "one-per-line"}),
        bool lower = fire::arg({"l", "lower-case"})) {
    if(lower)
        for(string &f: files)
            transform(f.begin(), f.end(), f.begin(), [](char c){ return std::tolower(c); });

    for(size_t i = 0; i < files.size(); ++i) {
        if(i >= 1)
            cout << (lines ? "\n" : " ");
        cout << files[i];
    }
    cout << endl;

    return 0;
}

FIRE_POSITIONAL(fire_main)
