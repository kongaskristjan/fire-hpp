
#include <gtest/gtest.h>
#include "fire.h"

using namespace std;
using namespace fire;

void init_test_with_args(const vector<string> &args) {
    const char ** argv = new const char *[args.size()];
    for(size_t i = 0; i < args.size(); ++i)
        argv[i] = args[i].c_str();

    _init_args(args.size(), argv);

    delete [] argv;
}

TEST(Named, defaults) {
    init_test_with_args({"./run_tests"});

    EXPECT_EQ((int) Named("-i", 1), 1);
    EXPECT_NEAR((double) Named("-f", 2), 2, 1e-5);
    EXPECT_NEAR((double) Named("-f", 2.0), 2.0, 1e-5);
    EXPECT_EQ((string) Named("-s", "test"), "test");

    EXPECT_DEATH((int) Named("-i", 1.0), "");
    EXPECT_DEATH((int) Named("-i", "test"), "");
    EXPECT_DEATH((double) Named("-f", "test"), "");
    EXPECT_DEATH((string) Named("-s", 1), "");
    EXPECT_DEATH((string) Named("-s", 1.0), "");
}

TEST(Named, parsing) {
    init_test_with_args({"./run_tests", "-i", "1", "-f", "2.0", "-s", "test"});

    EXPECT_EQ((int) Named("-i", 2), 1);

    EXPECT_EQ((int) Named("-i"), 1);
    EXPECT_NEAR((double) Named("-i"), 1.0, 1e-5);
    EXPECT_NEAR((double) Named("-f"), 2.0, 1e-5);
    EXPECT_EQ((string) Named("-s"), "test");

    EXPECT_ANY_THROW((int) Named("-f"));
    EXPECT_ANY_THROW((int) Named("-s"));
    EXPECT_ANY_THROW((double) Named("-s"));

    EXPECT_DEATH((int) Named("-x"), "");
}
