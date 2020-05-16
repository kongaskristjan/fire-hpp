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
    constexpr int _failure_code = 1;

    void _assert(bool pass, const std::string &msg) {
        if (pass)
            return;

        if (!msg.empty())
            std::cerr << "Error: " << msg << std::endl;
        exit(_failure_code);
    }

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
        T value() const { _assert(_exists, "Accessing unassigned optional"); return _value; }
    };
#endif

    class _overview { // All members are static
        static std::unordered_map<std::string, std::string> _args;
        static int _main_argc;
        static bool _loose_query;

    public:
        static void check(bool dec_main_argc = true);
        template<typename T> static T check_return(const T &ret, bool dec_main_argc = true);
        static optional<std::string> steal(const std::string &key);
        static void init_args(int argc, const char **argv, int main_argc, bool loose_query);
    };

    std::unordered_map<std::string, std::string> _overview::_args;
    int _overview::_main_argc;
    bool _overview::_loose_query;

    void _overview::check(bool dec_main_argc) {
        _main_argc -= dec_main_argc;
        if (_loose_query || _main_argc > 0 || _args.empty())
            return;

        std::string invalid;
        for (const auto &it: _args)
            invalid += std::string(" ") + it.first;
        _assert(false, std::string("Invalid argument") + (invalid.size() > 1 ? "s" : "") + invalid);
    }

    template<typename T>
    T _overview::check_return(const T &ret, bool dec_main_argc) {
        check(dec_main_argc);
        return ret;
    }

    optional<std::string> _overview::steal(const std::string &key) {
        auto it = _args.find(key);
        if (it == _args.end())
            return optional<std::string>();

        optional<std::string> opt = it->second;
        if (!_loose_query)
            _args.erase(it);
        return opt;
    }

    void _overview::init_args(int argc, const char **argv, int main_argc, bool loose_query) {
        _main_argc = main_argc;
        _loose_query = loose_query;
        _assert(argc % 2 == 1, "All arguments don't have values");
        _args.clear();
        for (int i = 1; i < argc; i += 2) {
            std::string name = argv[i], value = argv[i + 1];
            _args[name] = value;
        }

        check(false);
    }

    template<typename R, typename ... Types>
    constexpr size_t _get_argument_count(R(*f)(Types ...)) {
        return sizeof...(Types);
    }

    using int_t = int;
    using float_t = double;
    using string_t = std::string;

    class named {
        std::string _name;

        optional<int_t> _int_value;
        optional<float_t> _float_value;
        optional<string_t> _string_value;

        template <typename T> optional<T> get() { T::unimplemented_function; } // no default function

    public:
        explicit named(std::string _name): _name(std::move(_name)) {}
        named(std::string _name, int_t _value): _name(std::move(_name)), _int_value(_value) {}
        named(std::string _name, float_t _value): _name(std::move(_name)), _float_value(_value) {}
        named(std::string _name, const string_t &_value): _name(std::move(_name)), _string_value(_value) {}

        operator optional<int_t>();
        operator optional<float_t>();
        operator optional<string_t>();
        operator int_t();
        operator float_t();
        operator string_t();
    };

    template <>
    optional<int_t> named::get<int_t>() {
        auto elem = _overview::steal(_name);
        if(elem.has_value()) {
            size_t last;
            bool success = true;
            int converted;
            try { converted = std::stoi(elem.value(), &last); }
            catch(std::logic_error &) { success = false; }

            _assert(success && last == elem.value().size() /* probably was floating point */,
                    std::string("value ") + elem.value() + " is not an integer");

            return _overview::check_return(converted);
        }

        return _overview::check_return(_int_value);
    }

    template <>
    optional<float_t> named::get<float_t>() {
        auto elem = _overview::steal(_name);
        if(elem) {
            try {
                return _overview::check_return(std::stold(elem.value()));
            } catch(std::logic_error &) {
                _assert(false, std::string("value ") + elem.value() + " is not a real number");
            }
        }

        if(_float_value.has_value()) return _overview::check_return(_float_value);
        if(_int_value.has_value()) return _overview::check_return(_int_value.value());
        return {};
    }

    template <>
    optional<string_t> named::get<string_t>() {
        auto elem = _overview::steal(_name);
        if(elem)
            return _overview::check_return(elem.value());

        return _overview::check_return(_string_value);
    }

    named::operator optional<int_t>() {
        _assert(! _int_value.has_value() && ! _float_value.has_value() && ! _string_value.has_value(),
                "Optional argument has default value");
        return _overview::check_return(get<int_t>());
    }

    named::operator optional<float_t>() {
        _assert(! _int_value.has_value() && ! _float_value.has_value() && ! _string_value.has_value(),
                "Optional argument has default value");
        return _overview::check_return(get<float_t>());
    }

    named::operator optional<string_t>() {
        _assert(! _int_value.has_value() && ! _float_value.has_value() && ! _string_value.has_value(),
                "Optional argument has default value");
        return _overview::check_return(get<string_t>());
    }

    named::operator int_t() {
        optional<int_t> val = get<int_t>();
        _assert(val.has_value(), std::string("Required argument ") + _name + " not provided");
        return val.value();
    }

    named::operator float_t() {
        optional<float_t> val = get<float_t>();
        _assert(val.has_value(), std::string("Required argument ") + _name + " not provided");
        return val.value();
    }

    named::operator string_t() {
        optional<string_t> val = get<string_t>();
        _assert(val.has_value(), std::string("Required argument ") + _name + " not provided");
        return val.value();
    }
}



#define FIRE(main_func) \
int main(int argc, const char ** argv) {\
    std::size_t main_argc = fire::_get_argument_count(main_func);\
    fire::_overview::init_args(argc, argv, main_argc, false);\
    return main_func();\
}

#endif
