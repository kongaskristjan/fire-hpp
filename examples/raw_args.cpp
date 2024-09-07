
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

int fired_main(fire::optional<int> value = fire::arg("--value")) {
    int argc = fire::raw_args.argc();
    char ** argv = fire::raw_args.argv();

    if(value.has_value())
        std::cout << "value: " << value.value() << std::endl;

    std::cout << "argc: " << argc << std::endl;
    std::cout << "argv:";
    for(int i = 0; i < argc; ++i)
        std::cout << " " << argv[i];
    std::cout << std::endl;
    return 0;
}

FIRE_ALLOW_UNUSED(fired_main, "Prints raw argv/argc and if specified, the optional integer.")
