
/*
    Copyright (c) 2020 Kristjan Kongas

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
    REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
    AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
    INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
    LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
    OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
    PERFORMANCE OF THIS SOFTWARE.
*/

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

FIRE_NO_SPACE_ASSIGNMENT(fired_main)
