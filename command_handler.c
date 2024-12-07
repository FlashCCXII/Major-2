#include "command_handler.h"
#include "alias_handler.h"
#include "new_history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_LINE 1024
char line[MAX_LINE];

void handle_cd(char *path) {
    if (path == NULL) {
        // If no path is specified, change to HOME directory
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    }
    if (chdir(path) != 0) {
        // Handle errors from chdir
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
    }
}

void execute_command(char *command) {
    char *args[MAX_LINE / 2 + 1];
    char *token = strtok(command, " ");
    int i = 0;

    int input_redirect = -1;
    int output_redirect = -1;

    while (token && i < MAX_LINE / 2) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            input_redirect = open(token, O_RDONLY);
            if (input_redirect < 0) {
                perror("open for input");
                return;
            }
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            output_redirect = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_redirect < 0) {
                perror("open for output");
                return;
            }
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (args[0] == NULL) return;  // Empty command

    // Fork and execute external commands
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        
        // Handle redirection in the child process
        if (input_redirect != -1) {
            dup2(input_redirect, STDIN_FILENO);
            close(input_redirect);
        }

        if (output_redirect != -1) {
            dup2(output_redirect, STDOUT_FILENO);
            close(output_redirect);
        }

        if (execvp(args[0], args) < 0) {
            perror("execvp");
            exit(1);  // Exit child process on exec failure
        }
    } else {
        waitpid(pid, NULL, 0);
    }

    // Close any open file descriptors after use
    if (input_redirect != -1) {
        close(input_redirect);
    }
    if (output_redirect != -1) {
        close(output_redirect);
    }

    add_cmd(command);

    if (strcmp(args[0], "exit") == 0) {
        handle_exit(); 
    }
}

void handle_input(char *input) {
    char *args[MAX_LINE / 2 + 1];
    char *token = strtok(input, " ");
    int i = 0;

    if (is_alias(token)) {
        // Substitute the alias with the actual command
        execute_alias(token); 
        return;
    }

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (args[0] == NULL) return; // Empty line, do nothing

    if (strcmp(args[0], "exit") == 0) {
        handle_exit();
    } else if (strcmp(args[0], "cd") == 0) {
        handle_cd(args[1]);  // Ensure correct argument handling for cd
    } else if (strcmp(args[0], "alias") == 0) {
        handle_alias(args[1], args[2]);  // Correct arguments for alias
    } else if (is_alias(args[0])) {
        execute_alias(args[0]);  // Ensure alias is executed
    } else {
        execute_command(input);  // Call execute_command with input
    }

    if (strncmp(input, "myhistory", 9) == 0) {
        char *args = input + 9; // Skip "myhistory"
        while (*args == ' ') args++; // Trim leading spaces
        if (*args == '\0') {
            // Print the history
            print_history();
        } else if (strcmp(args, "-c") == 0) {
            // Clear the history
            clear_history();
        } else if (strncmp(args, "-e", 2) == 0) {
            // Execute a command from history
            args += 2;
            while (*args == ' ') args++; // Trim spaces
            int num = atoi(args);
            execute_history(num);
        } else {
            printf("Invalid myhistory usage\n");
        }
    }
}

