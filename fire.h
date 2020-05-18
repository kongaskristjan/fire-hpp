#ifndef FIRE_H
#define FIRE_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <cstdlib>
#include <algorithm>

#if __cplusplus >= 201500 // C++17
#include <optional>
#endif


namespace fire {
    constexpr int _failure_code = 1;

    void _instant_assert(bool pass, const std::string &msg) {
        if (pass)
            return;

        if (!msg.empty())
            std::cerr << "Error (programmer side): " << msg << std::endl;
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
        T value() const { _instant_assert(_exists, "Accessing unassigned optional"); return _value; }
    };
#endif

    class _matcher {
        static std::string _executable;
        static std::unordered_map<std::string, std::string> _args;
        static std::unordered_set<std::string> _queried;
        static std::vector<std::string> _deferred_errors;
        static int _main_argc;
        static bool _loose_query;
        static bool _help_flag;

    public:
        static void check(bool dec_main_argc);
        static optional<std::string> get_and_mark_as_queried(const std::string &key);
        static void init_args(int argc, const char **argv, int main_argc, bool loose_query);
        static const std::string& get_executable() { return _executable; }
        static bool deferred_assert(bool pass, const std::string &msg);
    };

    std::string _matcher::_executable;
    std::unordered_map<std::string, std::string> _matcher::_args;
    std::unordered_set<std::string> _matcher::_queried;
    std::vector<std::string> _matcher::_deferred_errors;
    int _matcher::_main_argc;
    bool _matcher::_loose_query;
    bool _matcher::_help_flag;

    class _help_logger { // Gathers function argument help info here
    public:
        struct log_elem {
            std::string descr;
            std::string type;
            std::string def;
            bool optional;
        };

    private:
        static std::map<std::string, log_elem> _params;

        static std::string _make_printable(const std::string &name, const log_elem &elem);
        static void _add_to_help(std::string &usage, std::string &options,
                                 const std::string &name, const log_elem &elem, size_t margin);
    public:
        static void print_help();
        static void log(const std::string &name, const log_elem &elem);
        static void clear() { _params.clear(); }
    };

    std::map<std::string, _help_logger::log_elem> _help_logger::_params;

    void _matcher::check(bool dec_main_argc) {
        _main_argc -= dec_main_argc;
        if(_loose_query || _main_argc > 0) return;

        if(_help_flag) {
            _help_logger::print_help();
            exit(0);
        }

        if(! _args.empty()) {
            std::string invalid;
            for (const auto &it: _args)
                invalid += std::string(" ") + it.first;
            deferred_assert(false, std::string("Invalid argument") + (invalid.size() > 1 ? "s" : "") + invalid);
        }

        if(! _deferred_errors.empty()) {
            std::cerr << "Error: " << _deferred_errors[0] << std::endl;
            exit(_failure_code);
        }
    }

    optional<std::string> _matcher::get_and_mark_as_queried(const std::string &key) {
        if(! deferred_assert(_queried.find(key) == _queried.end(), std::string("double query for argument ") + key))
            return {};

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

    void _matcher::init_args(int argc, const char **argv, int main_argc, bool loose_query) {
        _main_argc = main_argc;
        _loose_query = loose_query;
        _args.clear();
        _queried.clear();
        _help_flag = false;
        _help_logger::clear();

        _executable = argv[0];
        int i = 1;
        while(i < argc) {
            std::string hyphened_name = argv[i];
            if(hyphened_name == "-h" || hyphened_name == "--help") {
                _help_flag = true;
                i += 1;
                continue;
            }

            if(! deferred_assert(argc >= i + 2, "all parameters must have values")) return;
            std::string value = argv[i + 1];
            size_t hyphens = count_hyphens(hyphened_name);
            std::string name = hyphened_name.substr(hyphens);

            _instant_assert(name.size() > 0, std::string("ill formed parameter ") + name);
            if(name.size() == 1)
                if(! deferred_assert(hyphens == 1, std::string("single character parameter ") + name +
                                              " must prefix exactly one hyphen: -" + name)) return;
            if(name.size() >= 2)
                if(! deferred_assert(hyphens == 2, std::string("multi-character parameter ") + name +
                                              " must prefix exactly two hyphens: --" + name)) return;

            _args[name] = value;
            i += 2;
        }

        check(false);
    }

    bool _matcher::deferred_assert(bool pass, const std::string &msg) {
        if(_loose_query) {
            _instant_assert(pass, msg);
            return pass;
        }
        if(! pass)
            _deferred_errors.push_back(msg);
        return pass;
    }

    std::string _help_logger::_make_printable(const std::string &name, const log_elem &elem) {
        _instant_assert(name.size() >= 1, "Internal error");
        std::string printable;
        if(elem.optional) printable += "[";
        printable += std::string((name.size() == 1) ? 1 : 2, '-');
        printable += name;
        printable += "=<";
        printable += elem.type;
        printable += ">";
        if(elem.optional) printable += "]";
        return printable;
    }

    void _help_logger::_add_to_help(std::string &usage, std::string &options,
                                    const std::string &name, const log_elem &elem, size_t margin) {
        std::string printable = _make_printable(name, elem);
        usage += " ";
        usage += printable;

        options += "      " + printable + std::string(2 + margin - printable.size(), ' ') + elem.descr;
        if(! elem.def.empty())
            options += std::string(" [default: ") + elem.def + "]";
        options += "\n";
    }

    void _help_logger::print_help() {
        std::string usage = "    Usage:\n      " + _matcher::get_executable();
        std::string options = "    Options:\n";

        size_t margin = 0;
        for(const auto& it: _params)
            margin = std::max(margin, _make_printable(it.first, it.second).size());

        for(const auto& it: _params)
            if(! it.second.optional)
                _add_to_help(usage, options, it.first, it.second, margin);
        for(const auto& it: _params)
            if(it.second.optional)
                _add_to_help(usage, options, it.first, it.second, margin);

        std::cerr << std::endl << usage << std::endl << std::endl << std::endl << options << std::endl;
    }

    void _help_logger::log(const std::string &name, const log_elem &_elem) {
        log_elem elem = _elem;
        elem.optional |= ! elem.def.empty();
        _params.insert({name, elem});
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

        void _check_name() const;
        template <typename T> optional<T> _get() { T::unimplemented_function; } // no default function
        template <typename T> optional<T> _convert_optional();
        template <typename T> T _convert();
        void _log(const std::string &type, bool optional);

    public:
        explicit named(std::string _name, std::string _descr = ""):
            _name(std::move(_name)), _descr(std::move(_descr)) { _check_name(); }
        named(std::string _name, std::string _descr, int_t _value):
            _name(std::move(_name)), _descr(std::move(_descr)), _int_value(_value) { _check_name(); }
        named(std::string _name, std::string _descr, float_t _value):
            _name(std::move(_name)), _descr(std::move(_descr)), _float_value(_value) { _check_name(); }
        named(std::string _name, std::string _descr, const string_t &_value):
            _name(std::move(_name)), _descr(std::move(_descr)), _string_value(_value) { _check_name(); }

        operator optional<int_t>() { _log("INTEGER", true); return _convert_optional<int_t>(); }
        operator optional<float_t>() { _log("REAL", true); return _convert_optional<float_t>(); }
        operator optional<string_t>() { _log("STRING", true); return _convert_optional<string_t>(); }
        operator int_t() { _log("INTEGER", false); return _convert<int_t>(); }
        operator float_t() { _log("REAL", false); return _convert<float_t>(); }
        operator string_t() { _log("STRING", false); return _convert<string_t>(); }
    };

    void named::_check_name() const {
        _instant_assert(count_hyphens(_name) == 0, std::string("argument ") + _name +
                                                   "declaration must not have prefix hyphens (these are added automatically");
    }

    template <>
    optional<int_t> named::_get<int_t>() {
        auto elem = _matcher::get_and_mark_as_queried(_name);
        if(elem.has_value()) {
            size_t last;
            bool success = true;
            int converted = 0;
            try { converted = std::stoi(elem.value(), &last); }
            catch(std::logic_error &) { success = false; }

            _matcher::deferred_assert(success && last == elem.value().size() /* != indicates floating point */,
                            std::string("value ") + elem.value() + " is not an integer");

            return converted;
        }

        return _int_value;
    }

    template <>
    optional<float_t> named::_get<float_t>() {
        auto elem = _matcher::get_and_mark_as_queried(_name);
        if(elem) {
            try {
                return std::stold(elem.value());
            } catch(std::logic_error &) {
                _matcher::deferred_assert(false, std::string("value ") + elem.value() + " is not a real number");
            }
        }

        if(_float_value.has_value()) return _float_value;
        if(_int_value.has_value()) return _int_value.value();
        return {};
    }

    template <>
    optional<string_t> named::_get<string_t>() {
        auto elem = _matcher::get_and_mark_as_queried(_name);
        if(elem)
            return elem.value();

        return _string_value;
    }

    template <typename T>
    optional<T> named::_convert_optional() {
        _matcher::deferred_assert(!_int_value.has_value() && !_float_value.has_value() && !_string_value.has_value(),
                        "Optional argument has default value");
        _matcher::check(true);
        return _get<T>();
    }

    template <typename T>
    T named::_convert() {
        optional<T> val = _get<T>();
        _matcher::deferred_assert(val.has_value(), std::string("Required argument ") + _name + " not provided");
        _matcher::check(true);
        return val.value_or(T());
    }

    void named::_log(const std::string &type, bool optional) {
        std::string def;
        if(_int_value.has_value()) def = std::to_string(_int_value.value());
        if(_float_value.has_value()) def = std::to_string(_float_value.value());
        if(_string_value.has_value()) def = _string_value.value();

        _help_logger::log(_name, {_descr, type, def, optional});
    }
}



#define FIRE(main_func) \
int main(int argc, const char ** argv) {\
    std::size_t main_argc = fire::_get_argument_count(main_func);\
    fire::_matcher::init_args(argc, argv, main_argc, false);\
    return main_func();\
}

#endif
