#!/bin/bash

# Define the logfile
LOGFILE="shell_test_log.txt"

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Clear the logfile
> "$LOGFILE"

# Function to execute a command, log the result, and add color
execute_and_log() {
    local test_number=$1
    local command=$2

    echo -n "Test $test_number: Running '$command'... " | tee -a "$LOGFILE"
    if bash -c "$command" >> "$LOGFILE" 2>&1; then
        echo -e "Test $test_number: ${GREEN}OK${NC}" | tee -a "$LOGFILE"
    else
        echo -e "Test $test_number: ${RED}KO${NC}" | tee -a "$LOGFILE"
    fi
    echo "-----------------------------------" >> "$LOGFILE"
}

# Test cases
execute_and_log 1 "echo Hello, \$USER"
execute_and_log 2 "echo This will always print && echo This too || echo This will not print"
execute_and_log 3 "false || echo Prints because previous command failed"
execute_and_log 4 "echo First && echo Second && echo Third"
execute_and_log 5 "echo \$USER | wc"
execute_and_log 6 "echo This is a test | grep test && echo Search successful || echo Search failed"
execute_and_log 7 "echo User '\$USER' logged in"
execute_and_log 8 "false && echo This will not print"
execute_and_log 9 "echo Count the words | wc -w | grep 3"
execute_and_log 10 "echo Begin && false || echo End"
execute_and_log 11 "echo Your username is \$USER and you are logged in"
execute_and_log 12 "echo The date is \$(date)"
execute_and_log 13 "[ -z \"\$VAR\" ] && echo VAR is empty || echo VAR is not empty"
execute_and_log 14 "grep nonexistent somefile.txt && echo Found || echo Not found"
execute_and_log 15 "echo Path: \$PATH and User: '\$USER'"

echo "All tests completed. Results logged in $LOGFILE."
