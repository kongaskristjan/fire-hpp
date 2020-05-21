#ifndef __FIRE_HPP__
#define __FIRE_HPP__

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

    template<typename R, typename ... Types>
    constexpr size_t _get_argument_count(R(*f)(Types ...)) { return sizeof...(Types); }

    void _instant_assert(bool pass, const std::string &msg);
    size_t count_hyphens(const std::string &s);

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
        optional<T>& operator=(const T& __value) { _value = __value; _exists = true; return *this; }
        explicit operator bool() const { return _exists; }
        bool has_value() const { return _exists; }
        T value_or(const T& def) const { return _exists ? _value : def; }
        T value() const { _instant_assert(_exists, "Accessing unassigned optional"); return _value; }
    };
#endif

    class identifier {
        optional<std::string> _short_name, _long_name;

        static void _check_name(const std::string &name);
    public:
        identifier(std::initializer_list<const char *> lst); // {short name, long name} or {name}
        identifier(const char *name): identifier{name} {}

        bool operator<(const identifier &other) const;
        bool overlaps(const identifier &other) const;
        bool contains(const std::string &name) const;
        std::string help() const;
        std::string longer() const;
    };

    using bool_t = bool;
    using int_t = int;
    using float_t = double;
    using string_t = std::string;

    class _matcher {
        static std::string _executable;
        static std::vector<std::pair<std::string, optional<std::string>>> _args;
        static std::vector<identifier> _queried;
        static std::vector<std::string> _deferred_errors;
        static int _main_argc;
        static bool _loose_query;
        static bool _help_flag;

    public:
        enum class arg_type { string_t, bool_t, none_t };

        static void check(bool dec_main_argc);
        static std::pair<std::string, arg_type> get_and_mark_as_queried(const identifier &id);
        static void init_args(int argc, const char **argv, int main_argc, bool loose_query);
        static void parse(int argc, const char **argv);
        static const std::string& get_executable() { return _executable; }
        static bool deferred_assert(bool pass, const std::string &msg);
    };

    std::string _matcher::_executable;
    std::vector<std::pair<std::string, optional<std::string>>> _matcher::_args;
    std::vector<identifier> _matcher::_queried;
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
        static std::vector<std::pair<identifier, log_elem>> _params;

        static std::string _make_printable(const identifier &id, const log_elem &elem, bool verbose);
        static void _add_to_help(std::string &usage, std::string &options,
                                 const identifier &id, const log_elem &elem, size_t margin);
    public:
        static void print_help();
        static void log(const identifier &name, const log_elem &elem);
        static void clear() { _params.clear(); }
    };

    std::vector<std::pair<identifier, _help_logger::log_elem>> _help_logger::_params;

    class arg {
        identifier _id;
        std::string _descr;

        optional<int_t> _int_value;
        optional<float_t> _float_value;
        optional<string_t> _string_value;

        template <typename T> optional<T> _get() { T::unimplemented_function; } // no default function
        template <typename T> optional<T> _convert_optional();
        template <typename T> T _convert();
        void _log(const std::string &type, bool optional);

    public:
        explicit arg(identifier _id, std::string _descr = ""):
            _id(std::move(_id)), _descr(std::move(_descr)) {}
        arg(identifier _id, std::string _descr, int_t _value):
            _id(std::move(_id)), _descr(std::move(_descr)), _int_value(_value) {}
        arg(identifier _id, std::string _descr, float_t _value):
            _id(std::move(_id)), _descr(std::move(_descr)), _float_value(_value) {}
        arg(identifier _id, std::string _descr, const string_t &_value):
            _id(std::move(_id)), _descr(std::move(_descr)), _string_value(_value) {}

        explicit arg(std::initializer_list<const char *> init, std::string _descr = ""):
            _id(init), _descr(std::move(_descr)) {}
        arg(std::initializer_list<const char *> init, std::string _descr, int_t _value):
            _id(init), _descr(std::move(_descr)), _int_value(_value) {}
        arg(std::initializer_list<const char *> init, std::string _descr, float_t _value):
            _id(init), _descr(std::move(_descr)), _float_value(_value) {}
        arg(std::initializer_list<const char *> init, std::string _descr, const string_t &_value):
            _id(init), _descr(std::move(_descr)), _string_value(_value) {}

        operator optional<int_t>() { _log("INTEGER", true); return _convert_optional<int_t>(); }
        operator optional<float_t>() { _log("REAL", true); return _convert_optional<float_t>(); }
        operator optional<string_t>() { _log("STRING", true); return _convert_optional<string_t>(); }
        operator bool();
        operator int_t() { _log("INTEGER", false); return _convert<int_t>(); }
        operator float_t() { _log("REAL", false); return _convert<float_t>(); }
        operator string_t() { _log("STRING", false); return _convert<string_t>(); }
    };

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


    void identifier::_check_name(const std::string &name) {
        _instant_assert(count_hyphens(name) == 0, "argument " + name +
        " hyphens must not prefix declaration");
    }

    identifier::identifier(std::initializer_list<const char *> lst) { // {short name, long name} or {name}
        std::vector<std::string> names;
        for(auto x: lst)
            names.emplace_back(x);

        _instant_assert(names.size() == 1 || names.size() == 2,
                "identifier must be initialized with 1 or 2 names when using initializer list");

        if(names.size() == 1) {
            _instant_assert(names[0].size() >= 1, "Name must contain at least one character");
            _check_name(names[0]);

            if (names[0].size() == 1)
                _short_name = names[0];
            else
                _long_name = names[0];
        } else {
            _instant_assert(names[0].size() == 1, "Short name must contain exactly one character");
            _instant_assert(names[1].size() >= 2, "Long name must contain at least two characters");

            _check_name(names[0]);
            _check_name(names[1]);

            _short_name = names[0];
            _long_name = names[1];
        }
    }

    bool identifier::operator<(const identifier &other) const {
        std::string name = _long_name.value_or(_short_name.value_or(""));
        std::string other_name = other._long_name.value_or(other._short_name.value_or(""));

        return name < other_name;
    }

    bool identifier::overlaps(const identifier &other) const {
        if(_long_name.has_value() && other._long_name.has_value())
            if(_long_name.value() == other._long_name.value())
                return true;
        if(_short_name.has_value() && other._short_name.has_value())
            if(_short_name.value() == other._short_name.value())
                return true;
        return false;
    }

    bool identifier::contains(const std::string &name) const {
        if(_short_name.has_value() && name == _short_name.value()) return true;
        if(_long_name.has_value() && name == _long_name.value()) return true;
        return false;
    }

    std::string identifier::help() const {
        if(! _short_name.has_value()) return "--" + _long_name.value();
        if(! _long_name.has_value()) return "-" + _short_name.value();
        return "-" + _short_name.value() + "|--" + _long_name.value();
    }

    std::string identifier::longer() const {
        if(_long_name.has_value())
            return "--" + _long_name.value();
        return "-" + _short_name.value();
    }


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
                invalid += " " + it.first;
            deferred_assert(false, std::string("Invalid argument") + (invalid.size() > 1 ? "s" : "") + invalid);
        }

        if(! _deferred_errors.empty()) {
            std::cerr << "Error: " << _deferred_errors[0] << std::endl;
            exit(_failure_code);
        }
    }

    std::pair<std::string, _matcher::arg_type> _matcher::get_and_mark_as_queried(const identifier &id) {
        for(const auto& it: _queried)
            deferred_assert(! it.overlaps(id), "double query for argument " + id.longer());

        if (!_loose_query)
            _queried.push_back(id);

        for(auto it = _args.begin(); it != _args.end(); ++it)
            if(id.contains(it->first)) {
                optional<std::string> result = it->second;
                if(! _loose_query)
                    _args.erase(it);
                if(result.has_value())
                    return {result.value(), arg_type::string_t};
                return {"", arg_type::bool_t};
            }

        return {"", arg_type::none_t};
    }

    void _matcher::init_args(int argc, const char **argv, int main_argc, bool loose_query) {
        _main_argc = main_argc;
        _loose_query = loose_query;
        _args.clear();
        _queried.clear();
        _help_flag = false;
        _help_logger::clear();

        parse(argc, argv);
    }

    void _matcher::parse(int argc, const char **argv) {
        _executable = argv[0];

        std::string last_name;
        for(int i = 1; i < argc; ++i) {
            std::string hyphened_name = argv[i];
            size_t hyphens = count_hyphens(hyphened_name);
            std::string name = hyphened_name.substr(hyphens);

            if(hyphens == 0) {
                deferred_assert(! last_name.empty(), "positional arguments are not supported yet");
                _args.emplace_back(last_name, name);
                last_name = "";
            } else {
                if(! last_name.empty())
                    _args.emplace_back(last_name, optional<std::string>());

                if (name.size() == 1)
                    if (!deferred_assert(hyphens == 1, "single character parameter " + name +
                                                       " must prefix exactly one hyphen: -" + name))
                        return;
                if (name.size() >= 2)
                    if (!deferred_assert(hyphens == 2, "multi-character parameter " + name +
                                                       " must prefix exactly two hyphens: --" + name))
                        return;
                last_name = name;
            }
        }

        if(! last_name.empty())
            _args.emplace_back(last_name, optional<std::string>());

        _help_flag = get_and_mark_as_queried(identifier({"h", "help"})).second == arg_type::bool_t;

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

    std::string _help_logger::_make_printable(const identifier &id, const log_elem &elem, bool verbose) {
        std::string printable;
        if(elem.optional || elem.type == "") printable += "[";
        printable += verbose ? id.help() : id.longer();
        if(elem.type != "") {
            printable += "=<";
            printable += elem.type;
            printable += ">";
        }
        if(elem.optional || elem.type == "") printable += "]";
        return printable;
    }

    void _help_logger::_add_to_help(std::string &usage, std::string &options,
                                    const identifier &id, const log_elem &elem, size_t margin) {
        usage += " ";
        usage += _make_printable(id, elem, false);

        std::string printable = _make_printable(id, elem, true);
        options += "      " + printable + std::string(2 + margin - printable.size(), ' ') + elem.descr;
        if(! elem.def.empty())
            options += " [default: " + elem.def + "]";
        options += "\n";
    }

    void _help_logger::print_help() {
        using id2elem = std::pair<identifier, log_elem>;

        std::string usage = "    Usage:\n      " + _matcher::get_executable();
        std::string options = "    Options:\n";

        std::vector<std::pair<identifier, log_elem>> printed(_params);
        std::sort(printed.begin(), printed.end(), [](const id2elem &a, const id2elem &b) {
            if(a.second.optional != b.second.optional)
                return a.second.optional < b.second.optional;
            return a.first < b.first;
        });

        size_t margin = 0;
        for(const auto& it: printed)
            margin = std::max(margin, _make_printable(it.first, it.second, true).size());

        for(const auto& it: printed)
            if(! it.second.optional)
                _add_to_help(usage, options, it.first, it.second, margin);
        for(const auto& it: printed)
            if(it.second.optional)
                _add_to_help(usage, options, it.first, it.second, margin);

        std::cerr << std::endl << usage << std::endl << std::endl << std::endl << options << std::endl;
    }

    void _help_logger::log(const identifier &name, const log_elem &_elem) {
        log_elem elem = _elem;
        elem.optional |= ! elem.def.empty();
        _params.emplace_back(name, elem);
    }

    template <>
    optional<int_t> arg::_get<int_t>() {
        auto elem = _matcher::get_and_mark_as_queried(_id);
        _matcher::deferred_assert(elem.second != _matcher::arg_type::bool_t,
                                  "argument " + elem.first + " must have value");
        if(elem.second == _matcher::arg_type::string_t) {
            size_t last = 0;
            bool success = true;
            int converted = 0;
            try { converted = std::stoi(elem.first, &last); }
            catch(std::logic_error &) { success = false; }

            _matcher::deferred_assert(success && last == elem.first.size(), // != indicates floating point
                                      "value " + elem.first + " is not an integer");

            return converted;
        }

        return _int_value;
    }

    template <>
    optional<float_t> arg::_get<float_t>() {
        auto elem = _matcher::get_and_mark_as_queried(_id);
        _matcher::deferred_assert(elem.second != _matcher::arg_type::bool_t,
                "argument " + elem.first + " must have value");
        if(elem.second == _matcher::arg_type::string_t) {
            try {
                return std::stold(elem.first);
            } catch(std::logic_error &) {
                _matcher::deferred_assert(false, "value " + elem.first + " is not a real number");
            }
        }

        if(_float_value.has_value()) return _float_value;
        if(_int_value.has_value()) return _int_value.value();
        return {};
    }

    template <>
    optional<string_t> arg::_get<string_t>() {
        auto elem = _matcher::get_and_mark_as_queried(_id);
        _matcher::deferred_assert(elem.second != _matcher::arg_type::bool_t,
                                  "argument " + elem.first + " must have value");

        if(elem.second == _matcher::arg_type::string_t)
            return elem.first;
        return _string_value;
    }

    template <typename T>
    optional<T> arg::_convert_optional() {
        _matcher::deferred_assert(!_int_value.has_value() && !_float_value.has_value() && !_string_value.has_value(),
                        "Optional argument has default value");
        _matcher::check(true);
        return _get<T>();
    }

    template <typename T>
    T arg::_convert() {
        optional<T> val = _get<T>();
        _matcher::deferred_assert(val.has_value(), "Required argument " + _id.longer() + " not provided");
        _matcher::check(true);
        return val.value_or(T());
    }

    void arg::_log(const std::string &type, bool optional) {
        std::string def;
        if(_int_value.has_value()) def = std::to_string(_int_value.value());
        if(_float_value.has_value()) def = std::to_string(_float_value.value());
        if(_string_value.has_value()) def = _string_value.value();

        _help_logger::log(_id, {_descr, type, def, optional});
    }

    arg::operator bool() {
        _instant_assert(!_int_value.has_value() && !_float_value.has_value() && !_string_value.has_value(),
                _id.longer() + " flag parameter must not have default value");

        _log("", false); // User sees this as flag, not boolean option
        auto elem = _matcher::get_and_mark_as_queried(_id);
        _matcher::deferred_assert(elem.second != _matcher::arg_type::string_t,
                                  "flag " + elem.first + " must not have value");
        _matcher::check(true);
        return elem.second == _matcher::arg_type::bool_t;
    }
}



#define FIRE(main_func) \
int main(int argc, const char ** argv) {\
    std::size_t main_argc = fire::_get_argument_count(main_func);\
    fire::_matcher::init_args(argc, argv, main_argc, false);\
    return main_func();\
}

#endif
