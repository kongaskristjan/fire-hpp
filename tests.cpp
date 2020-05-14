
#include <gtest/gtest.h>
#include "fire.h"

using namespace std;
using namespace fire;

void init_args_with_strings(const vector<string> &args) {
    const char ** argv = new const char *[args.size()];
    for(size_t i = 0; i < args.size(); ++i)
        argv[i] = args[i].c_str();

    _init_args(args.size(), argv);

    delete [] argv;
}

TEST(Named, defaults) {
    init_args_with_strings({"./run_tests"});
    EXPECT_EQ((int) Named("-i", 1), 1);
    EXPECT_NEAR((double) Named("-f", 2), 2, 1e-5);
    EXPECT_NEAR((double) Named("-f", 2.0), 2.0, 1e-5);
    EXPECT_EQ((string) Named("-s", "test"), "test");
}

TEST(Named, parsing) {
    init_args_with_strings({"./run_tests", "-i", "1", "-f", "2.0", "-s", "test"});
    EXPECT_EQ((int) Named("-i"), 1);
    EXPECT_NEAR((double) Named("-i"), 1.0, 1e-5);
    EXPECT_NEAR((double) Named("-f"), 2.0, 1e-5);
    EXPECT_EQ((string) Named("-s"), "test");

    EXPECT_EQ((int) Named("-i", 2), 1);
}