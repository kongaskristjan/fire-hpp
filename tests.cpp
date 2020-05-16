
#include <gtest/gtest.h>
#include "fire.h"

#define EXPECT_EXIT_SUCCESS(statement) EXPECT_EXIT(statement, ::testing::ExitedWithCode(0), "")
#define EXPECT_EXIT_FAIL(statement) EXPECT_EXIT(statement, ::testing::ExitedWithCode(_fire_failure_code), "")

using namespace std;
using namespace fire;

void init_args(const vector<string> &args, bool loose_query, int named_calls = 1000000) {
    const char ** argv = new const char *[args.size()];
    for(size_t i = 0; i < args.size(); ++i)
        argv[i] = args[i].c_str();

    _Overview::init_args(args.size(), argv, named_calls, loose_query);

    delete [] argv;
}

void init_args_loose_query(const vector<string> &args) {
    init_args(args, true);
}

void init_args_strict_query(const vector<string> &args, int named_calls) {
    init_args(args, false, named_calls);
}



TEST(Optional, value) {
    Optional<int> no_value;
    EXPECT_FALSE((bool) no_value);
    EXPECT_FALSE(no_value.has_value());
    EXPECT_EQ(no_value.value_or(3), 3);

    Optional<int> value(1);
    EXPECT_TRUE((bool) value);
    EXPECT_TRUE(value.has_value());
    EXPECT_EQ(value.value_or(3), 1);
}

TEST(Optional, assignment) {
    Optional<int> opt1, opt2 = 3, opt3(3);
    EXPECT_FALSE(opt1.has_value());
    EXPECT_TRUE(opt2.has_value());
    EXPECT_TRUE(opt3.has_value());
    opt1 = opt3;
    EXPECT_TRUE(opt1.has_value());
}

TEST(Optional, no_value) {
    Optional<int> opt;
    EXPECT_FALSE((bool) opt);
    EXPECT_FALSE(opt.has_value());
    EXPECT_EQ(opt.value_or(3), 3);
}


TEST(Named, defaults) {
    init_args_loose_query({"./run_tests"});

    EXPECT_EQ((int) Named("-i", 1), 1);
    EXPECT_NEAR((double) Named("-f", 2), 2, 1e-5);
    EXPECT_NEAR((double) Named("-f", 2.0), 2.0, 1e-5);
    EXPECT_EQ((string) Named("-s", "test"), "test");

    EXPECT_EXIT_FAIL((int) Named("-i", 1.0));
    EXPECT_EXIT_FAIL((int) Named("-i", "test"));
    EXPECT_EXIT_FAIL((double) Named("-f", "test"));
    EXPECT_EXIT_FAIL((string) Named("-s", 1));
    EXPECT_EXIT_FAIL((string) Named("-s", 1.0));
}

TEST(Named, correct_parsing) {
    init_args_loose_query({"./run_tests", "-i", "1", "-f", "2.0", "-s", "test"});

    EXPECT_EQ((int) Named("-i", 2), 1);

    EXPECT_EQ((int) Named("-i"), 1);
    EXPECT_NEAR((double) Named("-i"), 1.0, 1e-5);
    EXPECT_NEAR((double) Named("-f"), 2.0, 1e-5);
    EXPECT_EQ((string) Named("-s"), "test");

    EXPECT_EXIT_FAIL((int) Named("-f"));
    EXPECT_EXIT_FAIL((int) Named("-s"));
    EXPECT_EXIT_FAIL((double) Named("-s"));

    EXPECT_EXIT_FAIL((int) Named("-x"));
}

TEST(Named, incorrect_parsing) {
    EXPECT_EXIT_FAIL(init_args_loose_query({"./run_tests", "-i"}));
}

TEST(Named, strict_query) {
    EXPECT_EXIT_FAIL(init_args_strict_query({"./run_tests", "-i", "1"}, 0));

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    (int_t) Named("-i");
    EXPECT_EXIT_FAIL((int_t) Named("-x"));

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    EXPECT_EXIT_FAIL((int_t) Named("-x"));

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    EXPECT_EXIT_FAIL((int_t) Named("-x", 0));
}
