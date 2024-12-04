# Major-2
CSCE 3600 Major 2
Daniel Ajayi (Repo Owner, main.c, cd)
Destinee White (makefile, newhistory.c)
Noah Coon (alias_handler.c, command_handler.c)
Tumi Oladimeji (path, README)

# Project Organization
Due to the size of the project, we seperated the work based on our strengths. 
Daniel: Implemented the cd command, interactive & batch mode functionality, and ensured seamless handling of user input and file processing
Noah: Handled the parsing and exeution of commands using fork() and exec() system calls; implemented the exit command
Tumi: Implemented the path command for the PATH variable; implemented the ";" execution feature; README
Destinee: Implemented the myhistory command with features for viewing, clearing, and executing commands from history; Created the makefile

# Design Overview
The shell is implemented as a multi-file C program, which includes the following components:
main.c: Shell's entry point, handles interactive and batch modes.
command_handler.c: Manages parsiong and execution using fork(), exec(), and waitpad()
alias.c: Functionality for aliasing, allowing users to create shortcuts for complex commands.
newhistory.c: Implements the myhistory command to track user inputs and manage history.

# Complete Specification
Lines that didn't have any commands in between semicolons were ignored.
Commands seperated by semicolons were executed in the order they appeared, for sequential execution. For invalid commands, an error message is displayed and the shell continues to run.

# Known Bugs or Problems
There may be some issues with pipelining, as support for more than 3 pipes had not been implemented.
Due to the requirements of the project, Ctrl-C and Ctrl-Z are ignored for the shell process, and child processes may not be able to handle these signals.
