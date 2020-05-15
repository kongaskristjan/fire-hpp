#!/usr/bin/python3

import os, subprocess

fire_failure_code = 1

class Assert_runner:
    test_count = 0
    check_count = 0

    def __init__(self, pth):
        self.pth = pth
        Assert_runner.test_count += 1

    def equal(self, cmd, out):
        result = subprocess.run([self.pth] + cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode == 0
        assert self.b2str(result.stdout.strip()) == out.strip()
        assert result.stderr == b""
        Assert_runner.check_count += 1

    def handled_failure(self, cmd):
        result = subprocess.run([self.pth] + cmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode == fire_failure_code
        assert result.stdout == b""
        assert result.stderr != b""
        Assert_runner.check_count += 1

    def b2str(self, b):
        return str(b, "utf-8")


def run_basic(pth_prefix):
    pth = pth_prefix + "basic"
    runner = Assert_runner(pth)

    runner.equal("", "0 + 0 = 0")
    runner.equal("-x 3", "3 + 0 = 3")
    runner.equal("-y 4", "0 + 4 = 4")
    runner.equal("-x 3 -y 4", "3 + 4 = 7")
    runner.handled_failure("-x test")
    runner.handled_failure("-x")


def main():
    pth_prefix = os.path.dirname(__file__) + "/examples/"

    print("Running tests in {} ...".format(pth_prefix), end="")
    run_basic(pth_prefix)
    print(" SUCCESS! (ran {} tests with {} checks)".format(Assert_runner.test_count, Assert_runner.check_count))


if __name__ == "__main__":
    main()
