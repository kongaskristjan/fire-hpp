#!/usr/bin/python3

import os, subprocess
from pathlib import Path

fire_failure_code = 1

class assert_runner:
    test_count = 0
    check_count = 0

    def __init__(self, pth):
        self.pth = pth
        assert_runner.test_count += 1

    def equal(self, cmd, out):
        result = subprocess.run([self.pth] + cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode == 0
        assert self.b2str(result.stdout.strip()) == out.strip()
        assert result.stderr == b""
        assert_runner.check_count += 1

    def handled_failure(self, cmd):
        result = subprocess.run([self.pth] + cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode == fire_failure_code
        assert result.stdout == b""
        assert result.stderr != b""
        assert_runner.check_count += 1

    def help_success(self, cmd):
        result = subprocess.run([self.pth] + cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode == 0
        assert_runner.check_count += 1

    def b2str(self, b):
        return str(b, "utf-8")


def run_all_combinations(pth_prefix):
    runner = assert_runner(pth_prefix + "all_combinations")

    runner.help_success("-h")


def run_basic(pth_prefix):
    runner = assert_runner(pth_prefix + "basic")

    runner.equal("", "0 + 0 = 0")
    runner.equal("-x 3", "3 + 0 = 3")
    runner.equal("-y 4", "0 + 4 = 4")
    runner.equal("-x 3 -y 4", "3 + 4 = 7")
    runner.equal("-x -3 -y 3", "-3 + 3 = 0")
    runner.equal("--x-coord 3 --y-coord 4", "3 + 4 = 7")
    runner.handled_failure("-x test")
    runner.handled_failure("-x")
    runner.handled_failure("--undefined 0")
    runner.help_success("-h")
    runner.help_success("--help")
    runner.help_success("-x 0 -h")
    runner.help_success("-h -x 0")


def run_flags(pth_prefix):
    runner = assert_runner(pth_prefix + "flags")

    runner.help_success("-h")
    runner.equal("", "0 0")
    runner.equal("-a -b", "1 1")
    runner.handled_failure("-a 1")


def run_positional(pth_prefix):
    runner = assert_runner(pth_prefix + "positional")

    runner.help_success("-h")
    runner.handled_failure("")
    runner.handled_failure("test")
    runner.equal("2", "2 0")
    runner.equal("2 3", "2 3")
    runner.handled_failure("2 3 4")
    runner.equal("-1 -3", "-1 -3")


def run_vector_positional(pth_prefix):
    runner = assert_runner(pth_prefix + "vector_positional")

    runner.help_success("-h")
    runner.equal("", "\n")
    runner.equal("b a", "b a\n")
    runner.equal("b a -o", "b\na\n")
    runner.equal("b a -s", "a b\n")
    runner.equal("b a -os", "a\nb\n")


def main():
    pth_prefix = str(Path(__file__).parent.parent / "examples") + "/"

    print("Running tests in {} ...".format(pth_prefix), end="")

    run_all_combinations(pth_prefix)
    run_basic(pth_prefix)
    run_flags(pth_prefix)
    run_positional(pth_prefix)
    run_vector_positional(pth_prefix)

    print(" SUCCESS! (ran {} tests with {} checks)".format(assert_runner.test_count, assert_runner.check_count))


if __name__ == "__main__":
    main()
