#!/usr/bin/python3

import sys, subprocess
from pathlib import Path
from termcolor import colored

def run(cmd):
    print("Running " + cmd)
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print(colored("Test failed", "red"))
        sys.exit(1)

def main():
    pth_prefix = str(Path(__file__).absolute().parent.parent) + "/"
    run(pth_prefix + "tests/run_tests")
    run(pth_prefix + "tests/run_examples.py")
    run(pth_prefix + "tests/link_test")
    print(colored("All tests returned SUCCESS!", "green"))


if __name__ == "__main__":
    main()
