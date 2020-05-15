#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <cstdlib>

namespace fire {
    constexpr int _fire_failure_code = 1;
    int _main_argc = 0;
    std::unordered_map<std::string, std::string> _args;
    bool _loose_query = false;

    template <typename R, typename ... Types> constexpr size_t _get_argument_count(R(*f)(Types ...)) {
        return sizeof...(Types);
    }

    struct _Steal_element {
        std::string key, value;
        bool exists = false;

        _Steal_element(std::unordered_map<std::string, std::string> &mp, const std::string &_key): key(_key) {
            auto it = mp.find(key);
            if(it == mp.end())
                return;

            value = it->second;
            exists = true;
            if(! _loose_query)
                mp.erase(it);
        }

        operator bool() { return exists; }
    };

    void _fire_assert(bool pass, const std::string &msg) {
        if(pass)
            return;

        if(! msg.empty())
            std::cerr << "Error: " << msg << std::endl;
        exit(_fire_failure_code);
    }

    void _check(bool dec_main_argc = true) {
        _main_argc -= dec_main_argc;
        if(_loose_query || _main_argc > 0 || _args.empty())
            return;

        std::string invalid;
        for(const auto &it: _args)
            invalid += std::string(" ") + it.first;
        _fire_assert(false, std::string("Invalid argument") + (invalid.size() > 1 ? "s" : "") + invalid);
    }

    template<typename T>
    T _check_return(const T &ret, bool dec_main_argc = true) {
        _check(dec_main_argc);
        return ret;
    }

    void _init_args(int argc, const char ** argv, int main_argc) {
        _main_argc = main_argc;
        _fire_assert(argc % 2 == 1, "All arguments don't have values");
        _args.clear();
        for(int i = 1; i < argc; i += 2) {
            std::string name = argv[i], value = argv[i + 1];
            _args[name] = value;
        }

        _check(false);
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

        operator int_t() {
            auto elem = _Steal_element(_args, name);
            if(elem) {
                size_t last;
                bool success = true;
                int int_val = 0;
                try { int_val = std::stoi(elem.value, &last); }
                catch(std::logic_error &) { success = false; }

                _fire_assert(success && last == elem.value.size() /* probably was floating point */,
                             std::string("value ") + elem.value + " is not an integer");

                return _check_return(int_val);
            }

            if(assigned == Type::int_t)
                return _check_return(value_int);
            _fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator float_t() {
            auto elem = _Steal_element(_args, name);
            if(elem) {
                try {
                    return _check_return(std::stold(elem.value));
                } catch(std::logic_error &) {
                    _fire_assert(false, std::string("value ") + elem.value + " is not a real number");
                }
            }

            if(assigned == Type::int_t)
                return _check_return(value_int);
            if(assigned == Type::float_t)
                return _check_return(value_float);
            _fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator string_t() {
            auto elem = _Steal_element(_args, name);
            auto it = _args.find(name);
            if(elem)
                return _check_return(elem.value);

            if(assigned == Type::string_t)
                return _check_return(value_string);
            _fire_assert(false, std::string("Error: required argument ") + name + " not provided");
            return "";
        }
    };
}



#define FIRE(main_func) \
int main(int argc, const char ** argv) {\
    std::size_t main_argc = fire::_get_argument_count(main_func);\
    fire::_init_args(argc, argv, main_argc);\
    return main_func();\
}

#endif
