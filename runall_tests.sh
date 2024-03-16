#!/bin/bash

# Save the current directory path
CURRENT_DIR=$(pwd)

make

# Move to tester directory which is assumed to be a subdirectory
echo "Moving to tester directory..."
cd minishell_tester
./tester ../Extras/test_cases_all

cd "$CURRENT_DIR"

echo "Running Norminette"
norminette | grep "Error\|Warning\|Global\|Notice"

