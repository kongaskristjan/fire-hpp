
#include <gtest/gtest.h>
#include "fire.h"

#define EXPECT_EXIT_CODE(statement, code) EXPECT_EXIT(statement, ::testing::ExitedWithCode(code), "")

using namespace std;
using namespace fire;

void init_args(const vector<string> &args, bool loose_query, int named_calls = 1000000) {
    const char ** argv = new const char *[args.size()];
    for(size_t i = 0; i < args.size(); ++i)
        argv[i] = args[i].c_str();

    _loose_query = loose_query;
    _init_args(args.size(), argv, named_calls);

    delete [] argv;
}

void init_args_loose_query(const vector<string> &args) {
    init_args(args, true);
}

void init_args_strict_query(const vector<string> &args, int named_calls) {
    init_args(args, false, named_calls);
}

TEST(Named, defaults) {
    init_args_loose_query({"./run_tests"});

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
    init_args_loose_query({"./run_tests", "-i", "1", "-f", "2.0", "-s", "test"});

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
    EXPECT_EXIT_CODE(init_args_loose_query({"./run_tests", "-i"}), _fire_failure_code);
}

TEST(Named, strict_query) {
    EXPECT_EXIT_CODE(init_args_strict_query({"./run_tests", "-i", "1"}, 0), _fire_failure_code);

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    (int_t) Named("-i");
    EXPECT_EXIT_CODE((int_t) Named("-x"), _fire_failure_code);

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    EXPECT_EXIT_CODE((int_t) Named("-x"), _fire_failure_code);

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    EXPECT_EXIT_CODE((int_t) Named("-x", 0), _fire_failure_code);
}
