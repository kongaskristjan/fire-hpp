#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <unordered_map>

namespace fire {
    struct _Arguments {
        static std::unordered_map<std::string, int> args;

    public:
        static void initArgs(int argc, char **argv) {
            for (int i = 1; i < argc; i += 2) {
                std::string name = argv[i];
                int value = atoi(argv[i + 1]);
                args[name] = value;
            }
        }
    };

    std::unordered_map<std::string, int> _Arguments::args;

    class Named {
        std::string name;
        std::optional<int> value;

    public:
        explicit Named(const std::string &_name) : name(_name) {
        }

        Named(const std::string &_name, int _value) : name(_name), value(_value) {
        }

        operator int() const {
            std::string __name = std::string("--") + name;
            if (auto it = _Arguments::args.find(__name); it != _Arguments::args.end())
                return it->second;
            return value.value();
        }
    };
}

#define FIRE(main_func) int main(int argc, char ** argv) { fire::_Arguments::initArgs(argc, argv); return main_func(); }

#endif
