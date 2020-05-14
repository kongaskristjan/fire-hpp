
#include <gtest/gtest.h>
#include "fire.h"

#define EXPECT_EXIT_CODE(statement, code) EXPECT_EXIT(statement, ::testing::ExitedWithCode(code), "")

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

    EXPECT_EXIT_CODE((int) Named("-i", 1.0), _fire_failure_code);
    EXPECT_EXIT_CODE((int) Named("-i", "test"), _fire_failure_code);
    EXPECT_EXIT_CODE((double) Named("-f", "test"), _fire_failure_code);
    EXPECT_EXIT_CODE((string) Named("-s", 1), _fire_failure_code);
    EXPECT_EXIT_CODE((string) Named("-s", 1.0), _fire_failure_code);
}

TEST(Named, correct_parsing) {
    init_test_with_args({"./run_tests", "-i", "1", "-f", "2.0", "-s", "test"});

    EXPECT_EQ((int) Named("-i", 2), 1);

    EXPECT_EQ((int) Named("-i"), 1);
    EXPECT_NEAR((double) Named("-i"), 1.0, 1e-5);
    EXPECT_NEAR((double) Named("-f"), 2.0, 1e-5);
    EXPECT_EQ((string) Named("-s"), "test");

    EXPECT_EXIT_CODE((int) Named("-f"), _fire_failure_code);
    EXPECT_EXIT_CODE((int) Named("-s"), _fire_failure_code);
    EXPECT_EXIT_CODE((double) Named("-s"), _fire_failure_code);

    EXPECT_EXIT_CODE((int) Named("-x"), _fire_failure_code);
}

TEST(Named, incorrect_parsing) {
    EXPECT_EXIT_CODE(init_test_with_args({"./run_tests", "-i"}), _fire_failure_code);
}
