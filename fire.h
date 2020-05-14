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

    using int_t = int;
    using float_t = double;
    using string_t = std::string;

    class Named {
        enum class Type { unassigned, int_t, float_t, string_t };

        std::string name;
        Type assigned = Type::unassigned;

        int_t value_int = 0;
        float_t value_float = 0;
        string_t value_string;

    public:
        explicit Named(std::string _name): name(std::move(_name)) {
        }

        Named(std::string _name, int_t _value): name(std::move(_name)),
            value_int(_value), assigned(Type::int_t) {
        }

        Named(std::string _name, float_t _value): name(std::move(_name)),
            value_float(_value), assigned(Type::float_t) {
        }

        Named(std::string _name, string_t _value): name(std::move(_name)),
            value_string(std::move(_value)), assigned(Type::string_t) {
        }

        operator int_t() const {
            auto it = _args.find(name);
            if(it != _args.end())
                return std::stoll(it->second.data());

            if(assigned == Type::int_t)
                return value_int;
            assert(false && "element not found in variables and no default value provided");
            return 0;
        }

        operator float_t() const {
            auto it = _args.find(name);
            if(it != _args.end())
                return std::stold(it->second.data());

            if(assigned == Type::int_t)
                return value_int;
            if(assigned == Type::float_t)
                return value_float;
            assert(false && "element not found in variables and no default value provided");
            return 0;
        }

        operator string_t() const {
            auto it = _args.find(name);
            if(it != _args.end())
                return it->second.data();

            if(assigned == Type::string_t)
                return value_string;
            assert(false && "element not found in variables and no default value provided");
            return 0;
        }
    };
}

#define FIRE(main_func) int main(int argc, char ** argv) { fire::_init_args(argc, argv); return main_func(); }

#endif
