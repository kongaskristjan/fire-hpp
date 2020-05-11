#include <iostream>
#include <unordered_map>

using namespace std;


struct _Arguments {
    static unordered_map<string, int> args;

public:
    static void initArgs(int argc, char ** argv) {
        for(int i = 1; i < argc; i += 2) {
            string name = argv[i];
            int value = atoi(argv[i + 1]);
            args[name] = value;
        }
    }
};

unordered_map<string, int> _Arguments::args;

class Named {
    string name;
    optional<int> value;

public:
    explicit Named(const string &_name): name(_name) {
    }

    Named(const string &_name, int _value): name(_name), value(_value) {
    }

    operator int() const {
        string __name = string("--") + name;
        if(auto it = _Arguments::args.find(__name); it != _Arguments::args.end())
            return it->second;
        return value.value();
    }
};

#define FIRE(main_func) int main(int argc, char ** argv) { _Arguments::initArgs(argc, argv); return main_func(); }


// --------------------------









int fire_main(int x = Named("x", 0), int y = Named("y", 0)) {
    cout << x << " + " << y << " = " << x + y << endl;
    return 0;
}

FIRE(fire_main)
