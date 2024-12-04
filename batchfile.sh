#!/bin/bash

# Test 1: Check the current directory (pwd)
echo "Testing pwd:"
pwd

# Test 2: Test cd with a directory
echo "Testing cd with a directory:"
cd /usr
pwd

# Test 3: Test cd with no argument (HOME directory)
echo "Testing cd with no argument (HOME):"
cd
pwd

# Test 4: Test exit with no other command
echo "Testing exit:"
exit

# Test 5: Test exit with another command (should run cat before exit)
echo "Testing exit with another command:"
cat /etc/passwd; exit

# Test 6: Test path command (show current PATH)
echo "Testing path command (show PATH):"
path

# Test 7: Test path command (append a new directory)
echo "Testing path command (append directory):"
path + /usr/local/bin
path

# Test 8: Test path command (remove a directory)
echo "Testing path command (remove directory):"
path - /usr/local/bin
path

# Test 9: Test myhistory command (show recent commands)
echo "Testing myhistory command:"
myhistory

# Test 10: Test myhistory with -c flag (clear history)
echo "Testing myhistory with -c flag:"
myhistory -c
myhistory

# Test 11: Test redirection (input redirection with <)
echo "Testing input redirection (with <):"
echo "This is a test input" > testfile.txt
< testfile.txt cat

# Test 12: Test redirection (output redirection with >)
echo "Testing output redirection (with >):"
echo "This is a test output" > outputfile.txt
cat outputfile.txt

# Test 13: Test multiple commands with exit at the end (exit should not affect execution of previous commands)
echo "Testing multiple commands with exit at the end:"
ls; exit; echo "This will not be printed"

echo "Test script completed."
