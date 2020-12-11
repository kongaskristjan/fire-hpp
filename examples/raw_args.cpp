
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
#include "fire-hpp/fire.hpp"

using namespace std;

int fired_main(std::vector<std::string> = fire::arg(fire::variadic())) {
    int argc = fire::original_args.argc();
    const char ** argv = fire::original_args.argv();

    std::cout << "argc: " << argc << std::endl;
    std::cout << "argv:";
    for(int i = 0; i < argc; ++i)
        std::cout << " " << argv[i];
    std::cout << std::endl;
    return 0;
}

FIRE(fired_main, "prints raw argv/argc")
