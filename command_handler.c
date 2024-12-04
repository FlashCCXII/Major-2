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

typedef struct {
    char alias[MAX_LINE];
    char command[MAX_LINE];
} Alias;

#define MAX_ALIASES 10
Alias aliases[MAX_ALIASES];
int alias_count = 0;

void handle_alias(char *alias, char *command) {
    if (alias_count < MAX_ALIASES) {
        strncpy(aliases[alias_count].alias, alias, MAX_LINE);
        strncpy(aliases[alias_count].command, command, MAX_LINE);
        alias_count++;
    } else {
        printf("Alias limit reached.\n");
    }
}

void execute_alias(char *alias) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].alias, alias) == 0) {
            printf("Executing alias: %s\n", aliases[i].command);
            handle_input(aliases[i].command);
            return;
        }
    }
    printf("Alias not found.\n");
}

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

    if (args[0] == NULL) return;

    // Alias substitution
    for (int j = 0; j < alias_count; j++) {
        if (strcmp(aliases[j].alias, args[0]) == 0) {
            printf("Substituting alias: %s -> %s\n", args[0], aliases[j].command);
            handle_input(aliases[j].command);
            return;
        }
    }

    // Fork and execute external commands
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
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
            exit(1);
        }
    } else {
        waitpid(pid, NULL, 0);
    }

    if (input_redirect != -1) {
    // Ensure the file exists before redirecting
    if (access(token, F_OK) != 0) {
        perror("open for input");
        return;
    }
    dup2(input_redirect, STDIN_FILENO);
    close(input_redirect);
	}

    if (output_redirect != -1) close(output_redirect);

    add_cmd(command);

    if (command=='exit'){
        handle_exit();
    }
}
