#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
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

    size_t count_hyphens(const std::string &s) {
        int hyphens;
        for(hyphens = 0; hyphens < s.size() && s[hyphens] == '-'; ++hyphens)
            ;
        return hyphens;
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
        static std::unordered_set<std::string> _queried;
        static int _main_argc;
        static bool _loose_query;

    public:
        static void check(bool dec_main_argc);
        static optional<std::string> get_and_mark_as_queried(const std::string &key);
        static void init_args(int argc, const char **argv, int main_argc, bool loose_query);
    };

    std::unordered_map<std::string, std::string> _overview::_args;
    std::unordered_set<std::string> _overview::_queried;
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

    optional<std::string> _overview::get_and_mark_as_queried(const std::string &key) {
        _assert(_queried.find(key) == _queried.end(), std::string("double query for argument ") + key);
        if (!_loose_query)
            _queried.insert(key);

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
        _queried.clear();
        for (int i = 1; i < argc; i += 2) {
            std::string hyphened_name = argv[i], value = argv[i + 1];
            size_t hyphens = count_hyphens(hyphened_name);
            std::string name = hyphened_name.substr(hyphens);

            _assert(name.size() > 0, std::string("ill formed parameter ") + name);
            if(name.size() == 1)
                _assert(hyphens == 1, std::string("single character parameter ") + name + " must prefix exactly one hyphen: -" + name);
            if(name.size() >= 2)
                _assert(hyphens == 2, std::string("multi-character parameter ") + name + " must prefix exactly two hyphens: --" + name);

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
        std::string _descr;

        optional<int_t> _int_value;
        optional<float_t> _float_value;
        optional<string_t> _string_value;

        void check_name() const;
        template <typename T> optional<T> get() { T::unimplemented_function; } // no default function
        template <typename T> optional<T> convert_optional();
        template <typename T> T convert();

    public:
        explicit named(std::string _name, std::string _descr = ""):
            _name(std::move(_name)), _descr(std::move(_descr)) { check_name(); }
        named(std::string _name, std::string _descr, int_t _value):
            _name(std::move(_name)), _descr(std::move(_descr)), _int_value(_value) { check_name(); }
        named(std::string _name, std::string _descr, float_t _value):
            _name(std::move(_name)), _descr(std::move(_descr)), _float_value(_value) { check_name(); }
        named(std::string _name, std::string _descr, const string_t &_value):
            _name(std::move(_name)), _descr(std::move(_descr)), _string_value(_value) { check_name(); }

        operator optional<int_t>() { return convert_optional<int_t>(); }
        operator optional<float_t>() { return convert_optional<float_t>(); }
        operator optional<string_t>() { return convert_optional<string_t>(); }
        operator int_t() { return convert<int_t>(); }
        operator float_t() { return convert<float_t>(); }
        operator string_t() { return convert<string_t>(); }
    };

    void named::check_name() const {
        _assert(count_hyphens(_name) == 0, std::string("argument ") + _name + "declaration must not have prefix hyphens (these are added automatically");
    }

    template <>
    optional<int_t> named::get<int_t>() {
        auto elem = _overview::get_and_mark_as_queried(_name);
        if(elem.has_value()) {
            size_t last;
            bool success = true;
            int converted;
            try { converted = std::stoi(elem.value(), &last); }
            catch(std::logic_error &) { success = false; }

            _assert(success && last == elem.value().size() /* probably was floating point */,
                    std::string("value ") + elem.value() + " is not an integer");

            return converted;
        }

        return _int_value;
    }

    template <>
    optional<float_t> named::get<float_t>() {
        auto elem = _overview::get_and_mark_as_queried(_name);
        if(elem) {
            try {
                return std::stold(elem.value());
            } catch(std::logic_error &) {
                _assert(false, std::string("value ") + elem.value() + " is not a real number");
            }
        }

        if(_float_value.has_value()) return _float_value;
        if(_int_value.has_value()) return _int_value.value();
        return {};
    }

    template <>
    optional<string_t> named::get<string_t>() {
        auto elem = _overview::get_and_mark_as_queried(_name);
        if(elem)
            return elem.value();

        return _string_value;
    }

    template <typename T>
    optional<T> named::convert_optional() {
        _assert(! _int_value.has_value() && ! _float_value.has_value() && ! _string_value.has_value(),
                "Optional argument has default value");
        _overview::check(true);
        return get<T>();
    }

    template <typename T>
    T named::convert() {
        optional<T> val = get<T>();
        _assert(val.has_value(), std::string("Required argument ") + _name + " not provided");
        _overview::check(true);
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
