#!/bin/bash

# Some programs in this repo (ex. r.filetype) communicate their result through
# the exit code.

# The expected exit code is the first argument, everything after is the command
# and its arguments
expected_exit_code=$1
shift

# Run the command and save the exit code
"$@"
result_exit_code=$?

# Compare the result to what is expected, the test fails if they are different
if ((expected_exit_code != result_exit_code)) ; then
    printf "Command '%s' was expected to exit with code %d but exited with code %d" "$*" $expected_exit_code $result_exit_code
    exit 1
fi
