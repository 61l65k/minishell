#!/bin/bash

# Save the current directory path
CURRENT_DIR=$(pwd)

# Move to tester directory which is assumed to be a subdirectory
echo "Moving to tester directory..."
cd minishell_tester

# Running tests
echo "Running basic all test!"
./tester
echo "Running tester syntax!"
./tester syntax
echo "Running builtins tests..."
./tester builtins
echo "Builtins tests completed."
echo "Running pipes tests..."
./tester pipes
echo "Pipes tests completed."
echo "Running redirects tests..."
./tester redirects
echo "Redirects tests completed."
echo "Running extras tests..."
./tester extras
echo "Extras tests completed."

# Go back to minishell directory and rename minishell for bonus tests
echo "Renaming minishell to minishell_bonus for bonus tests..."
cd "$CURRENT_DIR"
mv minishell minishell_bonus

# Go back to tester directory
cd minishell_tester

echo "Running wildcards tests with minishell_bonus..."
./tester wildcards
echo "Wildcards tests completed."
echo "Running bonus tests with minishell_bonus..."
./tester bonus
echo "Bonus tests completed."

# Renaming back to original after tests
cd "$CURRENT_DIR"
mv minishell_bonus minishell

echo "All tests completed."

echo "Running Norminette"
norminette

