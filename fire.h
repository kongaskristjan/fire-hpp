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
    using Optional = std::optional<T>;
#else // <= C++14
    template <typename T>
    class Optional {
        T _value;
        bool _exists = false;

    public:
        Optional() = default;
        Optional(const T &__value): _value(__value), _exists(true) {}
        Optional<T>& operator=(const T& __value) { _value = __value; _exists = true; }
        explicit operator bool() const { return _exists; }
        bool has_value() const { return _exists; }
        T value_or(const T& def) const { return _exists ? _value : def; }
    };
#endif

    class _Overview { // All members are static
        static std::unordered_map<std::string, std::string> _args;
        static int _main_argc;
        static bool _loose_query;

    public:
        struct Steal_element {
            std::string key, value;
            bool exists = false;

            Steal_element(std::string _key) : key(std::move(_key)) {
                auto it = _args.find(key);
                if (it == _args.end())
                    return;

                value = it->second;
                exists = true;
                if (!_loose_query)
                    _args.erase(it);
            }

            operator bool() { return exists; }
        };

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

    std::unordered_map<std::string, std::string> _Overview::_args;
    int _Overview::_main_argc;
    bool _Overview::_loose_query;

    template<typename R, typename ... Types>
    constexpr size_t _get_argument_count(R(*f)(Types ...)) {
        return sizeof...(Types);
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
            auto elem = _Overview::Steal_element(name);
            if(elem) {
                size_t last;
                bool success = true;
                int int_val = 0;
                try { int_val = std::stoi(elem.value, &last); }
                catch(std::logic_error &) { success = false; }

                _Overview::fire_assert(success && last == elem.value.size() /* probably was floating point */,
                                       std::string("value ") + elem.value + " is not an integer");

                return _Overview::check_return(int_val);
            }

            if(assigned == Type::int_t)
                return _Overview::check_return(value_int);
            _Overview::fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator float_t() {
            auto elem = _Overview::Steal_element(name);
            if(elem) {
                try {
                    return _Overview::check_return(std::stold(elem.value));
                } catch(std::logic_error &) {
                    _Overview::fire_assert(false, std::string("value ") + elem.value + " is not a real number");
                }
            }

            if(assigned == Type::int_t)
                return _Overview::check_return(value_int);
            if(assigned == Type::float_t)
                return _Overview::check_return(value_float);
            _Overview::fire_assert(false, std::string("required argument ") + name + " not provided");
            return 0;
        }

        operator string_t() {
            auto elem = _Overview::Steal_element(name);
            if(elem)
                return _Overview::check_return(elem.value);

            if(assigned == Type::string_t)
                return _Overview::check_return(value_string);
            _Overview::fire_assert(false, std::string("Error: required argument ") + name + " not provided");
            return "";
        }
    };
}



#define FIRE(main_func) \
int main(int argc, const char ** argv) {\
    std::size_t main_argc = fire::_get_argument_count(main_func);\
    fire::_Overview::init_args(argc, argv, main_argc, false);\
    return main_func();\
}

#endif
