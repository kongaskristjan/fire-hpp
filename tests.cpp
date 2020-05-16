
#include <gtest/gtest.h>
#include "fire.h"

#define EXPECT_EXIT_SUCCESS(statement) EXPECT_EXIT(statement, ::testing::ExitedWithCode(0), "")
#define EXPECT_EXIT_FAIL(statement) EXPECT_EXIT(statement, ::testing::ExitedWithCode(fire::_failure_code), "")

using namespace std;
using namespace fire;

void init_args(const vector<string> &args, bool loose_query, int named_calls = 1000000) {
    const char ** argv = new const char *[args.size()];
    for(size_t i = 0; i < args.size(); ++i)
        argv[i] = args[i].c_str();

    _overview::init_args(args.size(), argv, named_calls, loose_query);

    delete [] argv;
}

void init_args_loose_query(const vector<string> &args) {
    init_args(args, true);
}

void init_args_strict_query(const vector<string> &args, int named_calls) {
    init_args(args, false, named_calls);
}



TEST(optional, value) {
    fire::optional<int> no_value;
    EXPECT_FALSE((bool) no_value);
    EXPECT_FALSE(no_value.has_value());
    EXPECT_EQ(no_value.value_or(3), 3);
    EXPECT_EXIT_FAIL(no_value.value());

    fire::optional<int> value(1);
    EXPECT_TRUE((bool) value);
    EXPECT_TRUE(value.has_value());
    EXPECT_EQ(value.value_or(3), 1);
    EXPECT_EQ(value.value(), 1);
}

TEST(optional, assignment) {
    fire::optional<int> opt1, opt2 = 3, opt3(3);
    EXPECT_FALSE(opt1.has_value());
    EXPECT_TRUE(opt2.has_value());
    EXPECT_TRUE(opt3.has_value());
    opt1 = opt3;
    EXPECT_TRUE(opt1.has_value());
}

TEST(optional, no_value) {
    fire::optional<int> opt;
    EXPECT_FALSE((bool) opt);
    EXPECT_FALSE(opt.has_value());
    EXPECT_EQ(opt.value_or(3), 3);
}


TEST(init_args, invalid_input) {
    EXPECT_EXIT_FAIL(init_args_loose_query({"./run_tests", "-i"}));
    EXPECT_EXIT_FAIL(init_args_loose_query({"./run_tests", "--i", "0"}));
    EXPECT_EXIT_FAIL(init_args_loose_query({"./run_tests", "-int", "0"}));
}


TEST(named, false_hyphens) {
    init_args_loose_query({"./run_tests"});

    EXPECT_EXIT_FAIL(named("--undefined"));
    EXPECT_EXIT_FAIL(named("-i"));
    EXPECT_EXIT_FAIL(named("-i", 0));
    EXPECT_EXIT_FAIL(named("-f", 0.0));
    EXPECT_EXIT_FAIL(named("-s", "test"));
}

TEST(named, defaults) {
    init_args_loose_query({"./run_tests"});

    EXPECT_EQ((int) named("i", 1), 1);
    EXPECT_NEAR((double) named("f", 2), 2, 1e-5);
    EXPECT_NEAR((double) named("f", 2.0), 2.0, 1e-5);
    EXPECT_EQ((string) named("s", "test"), "test");

    EXPECT_EXIT_FAIL((int) named("i", 1.0));
    EXPECT_EXIT_FAIL((int) named("i", "test"));
    EXPECT_EXIT_FAIL((double) named("f", "test"));
    EXPECT_EXIT_FAIL((string) named("s", 1));
    EXPECT_EXIT_FAIL((string) named("s", 1.0));
}

TEST(named, correct_parsing) {
    init_args_loose_query({"./run_tests", "-i", "1", "-f", "2.0", "-s", "test"});

    EXPECT_EQ((int) named("i", 2), 1);

    EXPECT_EQ((int) named("i"), 1);
    EXPECT_NEAR((double) named("i"), 1.0, 1e-5);
    EXPECT_NEAR((double) named("f"), 2.0, 1e-5);
    EXPECT_EQ((string) named("s"), "test");

    EXPECT_EXIT_FAIL((int) named("f"));
    EXPECT_EXIT_FAIL((int) named("s"));
    EXPECT_EXIT_FAIL((double) named("s"));

    EXPECT_EXIT_FAIL((int) named("x"));
}

TEST(named, strict_query) {
    EXPECT_EXIT_FAIL(init_args_strict_query({"./run_tests", "-i", "1"}, 0));

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    (int_t) named("i");
    EXPECT_EXIT_FAIL((int_t) named("x"));

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    EXPECT_EXIT_FAIL((int_t) named("x"));

    init_args_strict_query({"./run_tests", "-i", "1"}, 1);
    EXPECT_EXIT_FAIL((int_t) named("x", 0));
}

TEST(named, optional_arguments) {
    init_args_loose_query({"./run_tests", "-i", "1", "-f", "1.0", "-s", "test"});

    fire::optional<int_t> i_undef = named("undefined");
    fire::optional<int_t> i = named("i");
    EXPECT_FALSE(i_undef.has_value());
    EXPECT_EQ(i.value(), 1);

    fire::optional<float_t> f_undef = named("undefined");
    fire::optional<float_t> f = named("f");
    EXPECT_FALSE(f_undef.has_value());
    EXPECT_NEAR(f.value(), 1.0, 1e-5);

    fire::optional<string_t> s_undef = named("undefined");
    fire::optional<string_t> s = named("s");
    EXPECT_FALSE(s_undef.has_value());
    EXPECT_EQ(s.value(), "test");
}

TEST(named, optional_and_default) {
    init_args_loose_query({"./run_tests", "-i", "0"});

    EXPECT_EXIT_FAIL(fire::optional<int_t> i_undef = named("undefined", 0));
    EXPECT_EXIT_FAIL(fire::optional<int_t> i = named("i", 0));
}
