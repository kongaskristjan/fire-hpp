
/*
    Copyright (c) 2020-2024 Kristjan Kongas

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
#include "fire-hpp/fire.hpp"

using namespace std;

int fired_main(int x = fire::arg("-x").bounds(-1000, 1000),
               int y = fire::arg("-y").bounds(-1000, 1000),
               fire::optional<int> mul = fire::arg("--mul").one_of({-1, 0, 1})) {
    if(mul.has_value())
        cout << mul.value() << " * (" << x << " + " << y << ") = " << mul.value() * (x + y) << endl;
    else
        cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fired_main)
