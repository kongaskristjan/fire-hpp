#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <cstdlib>

#if __cplusplus >= 201500 // C++17
#include <optional>
#endif


namespace fire {
    constexpr int _fire_failure_code = 1;

#if __cplusplus >= 201500 // >= C++17
    template <typename T>
    using optional = std::optional<T>;
#else // <= C++14
    template <typename T>
    class optional {
        T _value;
        bool _exists = false;

    public:
        optional() = default;
        optional(const T &__value): _value(__value), _exists(true) {}
        optional<T>& operator=(const T& __value) { _value = __value; _exists = true; }
        explicit operator bool() const { return _exists; }
        bool has_value() const { return _exists; }
        T value_or(const T& def) const { return _exists ? _value : def; }
    };
#endif

    class _overview { // All members are static
        static std::unordered_map<std::string, std::string> _args;
        static int _main_argc;
        static bool _loose_query;

    public:
        static void fire_assert(bool pass, const std::string &msg) {
            if (pass)
                return;

            if (!msg.empty())
                std::cerr << "Error: " << msg << std::endl;
            exit(_fire_failure_code);
        }

        static void check(bool dec_main_argc = true) {
            _main_argc -= dec_main_argc;
            if (_loose_query || _main_argc > 0 || _args.empty())
                return;

            std::string invalid;
            for (const auto &it: _args)
                invalid += std::string(" ") + it.first;
            fire_assert(false, std::string("Invalid argument") + (invalid.size() > 1 ? "s" : "") + invalid);
        }

        template<typename T>
        static T check_return(const T &ret, bool dec_main_argc = true) {
            check(dec_main_argc);
            return ret;
        }

        static optional<std::string> steal(const std::string &key) {
            auto it = _args.find(key);
            if (it == _args.end())
                return optional<std::string>();

            optional<std::string> opt = it->second;
            if (!_loose_query)
                _args.erase(it);
            return opt;
        }

        static void init_args(int argc, const char **argv, int main_argc, bool loose_query) {
            _main_argc = main_argc;
            _loose_query = loose_query;
            fire_assert(argc % 2 == 1, "All arguments don't have values");
            _args.clear();
            for (int i = 1; i < argc; i += 2) {
                std::string name = argv[i], value = argv[i + 1];
                _args[name] = value;
            }

            check(false);
        }
    };

    std::unordered_map<std::string, std::string> _overview::_args;
    int _overview::_main_argc;
    bool _overview::_loose_query;

    template<typename R, typename ... Types>
    constexpr size_t _get_argument_count(R(*f)(Types ...)) {
        return sizeof...(Types);
    }

    using int_t = int;
    using float_t = double;
    using string_t = std::string;

    class named {
        enum class type { unassigned, int_t, float_t, string_t };

        std::string name;
        type assigned = type::unassigned;

        int_t value_int = 0;
        float_t value_float = 0;
        string_t value_string;

    public:
        explicit named(std::string _name): name(std::move(_name)) {
        }

        named(std::string _name, int_t _value): name(std::move(_name)),
            value_int(_value), assigned(type::int_t) {
        }

        named(std::string _name, float_t _value): name(std::move(_name)),
            value_float(_value), assigned(type::float_t) {
        }

        named(std::string _name, string_t _value): name(std::move(_name)),
            value_string(std::move(_value)), assigned(type::string_t) {
        }

        operator int_t() {
            auto elem = _overview::steal(name);
            if(elem.has_value()) {
                size_t last;
                bool success = true;
                int int_val = 0;
                try { int_val = std::stoi(elem.value_or(""), &last); }
                catch(std::logic_error &) { success = false; }

                _overview::fire_assert(success && last == elem.value_or("").size() /* probably was floating point */,
                                       std::string("value ") + elem.value_or("") + " is not an integer");

                return _overview::check_return(int_val);
            }

            if(assigned == type::int_t)
                return _overview::check_return(value_int);
            _overview::fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator float_t() {
            auto elem = _overview::steal(name);
            if(elem) {
                try {
                    return _overview::check_return(std::stold(elem.value_or("")));
                } catch(std::logic_error &) {
                    _overview::fire_assert(false, std::string("value ") + elem.value_or("") + " is not a real number");
                }
            }

            if(assigned == type::int_t)
                return _overview::check_return(value_int);
            if(assigned == type::float_t)
                return _overview::check_return(value_float);
            _overview::fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator string_t() {
            auto elem = _overview::steal(name);
            if(elem)
                return _overview::check_return(elem.value_or(""));

            if(assigned == type::string_t)
                return _overview::check_return(value_string);
            _overview::fire_assert(false, std::string("Error: required argument ") + name + " not provided");
            return "";
        }
    };
}



#define FIRE(main_func) \
int main(int argc, const char ** argv) {\
    std::size_t main_argc = fire::_get_argument_count(main_func);\
    fire::_overview::init_args(argc, argv, main_argc, false);\
    return main_func();\
}

#endif
