cmake_minimum_required(VERSION 3.1)

set(JSON_STRING "{\"run_tests\": \"${RUN_TESTS_BUILD_DIR}\", \"examples\": \"${EXAMPLES_BUILD_DIR}\"}\n")
file(WRITE ${FILE_DIR}/build_dirs.json ${JSON_STRING})
