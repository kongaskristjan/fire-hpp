#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <unordered_map>
#include <cassert>

namespace fire {
    std::unordered_map<std::string, std::string> _args;

    void _init_args(int argc, char **argv) {
        for (int i = 1; i < argc; i += 2) {
            std::string name = argv[i], value = argv[i + 1];
            _args[name] = value;
        }
    }


    class Named {
        std::string name;
        int value;
        bool assigned = false;

    public:
        explicit Named(std::string _name) : name(std::move(_name)) {
        }

        Named(std::string _name, int _value) : name(std::move(_name)),
            value(_value), assigned(true) {
        }

        operator int() const {
            auto it = _args.find(name);
            if(it != _args.end())
                return atoi(it->second.data());
            if(assigned)
                return value;
            assert(false && "element not found in variables and no default value provided");
            return 0;
        }
    };
}

#define FIRE(main_func) int main(int argc, char ** argv) { fire::_init_args(argc, argv); return main_func(); }

#endif
