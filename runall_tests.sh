#!/bin/bash

# Save the current directory path
CURRENT_DIR=$(pwd)

# Move to tester directory which is assumed to be a subdirectory
echo "Moving to tester directory..."
cd minishell_tester

# Running tests
echo "Running non bonus tests..."
./tester builtins
./tester pipes
./tester redirects
./tester extras
./tester syntax

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
norminette | grep -v "OK!"

