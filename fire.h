#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <cstdlib>

namespace fire {
    constexpr int _fire_failure_code = 1;

    void _fire_assert(bool pass, const std::string &msg) {
        if(pass)
            return;

        if(! msg.empty())
            std::cerr << "Error: " << msg << std::endl;
        exit(_fire_failure_code);
    }

    std::unordered_map<std::string, std::string> _args;

    void _init_args(int argc, const char ** argv) {
        _fire_assert(argc % 2 == 1, "All arguments don't have values");
        _args.clear();
        for(int i = 1; i < argc; i += 2) {
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
            if(it != _args.end()) {
                const std::string str_value = it->second;
                size_t last;

                bool success = true;
                int value;
                try { value = std::stoi(str_value.data(), &last); }
                catch(std::logic_error) { success = false; }

                _fire_assert(success && last == str_value.size() /* probably was floating point */,
                             std::string("value ") + str_value + " is not an integer");

                return value;
            }

            if(assigned == Type::int_t)
                return value_int;
            _fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator float_t() const {
            auto it = _args.find(name);
            if (it != _args.end()) {
                try {
                    return std::stold(it->second.data());
                } catch (std::logic_error) {
                    _fire_assert(false, std::string("value ") + it->second + " is not a real number");
                }
            }

            if(assigned == Type::int_t)
                return value_int;
            if(assigned == Type::float_t)
                return value_float;
            _fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator string_t() const {
            auto it = _args.find(name);
            if(it != _args.end())
                return it->second.data();

            if(assigned == Type::string_t)
                return value_string;
            _fire_assert(false, std::string("Error: required argument ") + name + " not provided");
            return 0;
        }
    };
}

#define FIRE(main_func) int main(int argc, const char ** argv) { fire::_init_args(argc, argv); return main_func(); }

#endif
