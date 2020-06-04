
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
#include "../fire.hpp"

using namespace std;

int fired_main(
        int = fire::arg("i", "Integer"),
        double = fire::arg("r", "Real number"),
        string = fire::arg("s", "String"),
        int = fire::arg("def-i", "Default value integer", 1),
        double = fire::arg("def-r", "Default value real number", 1.0),
        string = fire::arg("def-s", "Default value string", "default text"),
        fire::optional<int> = fire::arg("opt-i", "Optional integer"),
        fire::optional<double> = fire::arg("opt-r", "Optional real number"),
        fire::optional<string> = fire::arg("opt-s", "Optional string"),
        unsigned = fire::arg(0),
        long double = fire::arg(1),
        string = fire::arg(2),
        fire::optional<int> = fire::arg(3),
        fire::optional<double> = fire::arg(4),
        fire::optional<string> = fire::arg(5)
        ) {
    return 0;
}

FIRE_NO_SPACE_ASSIGNMENT(fired_main)
