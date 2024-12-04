#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_LINE 512
#define HISTORY_SIZE 20

// Function Prototypes
void interactive_mode();
void batch_mode(const char *filename);
void execute_command(char *command);
void handle_cd(char *path);
void handle_exit();
void handle_path(char *args);
void handle_myhistory(char *args);

char *history[HISTORY_SIZE];
int history_count = 0;

int main(int argc, char *argv[]) {
    signal(SIGINT, SIG_IGN);   // Ignore Ctrl-C
    signal(SIGTSTP, SIG_IGN);  // Ignore Ctrl-Z

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [batchFile]\n", argv[0]);
        exit(1);
    }

    if (argc == 2) {
        batch_mode(argv[1]);
    } else {
        interactive_mode();
    }

    while (1){
        printf("> ");
        fgets(inpt, 1024, stdin);
        inpt[strlen(inpt)-1] = '\0';
        if(strcmp(inpt, "myhistory") == 0){
            print_hist();
        }else if (strncmp(inpt, "myhistory -e", 13) == 0){
            int num = atoi(inpt+13);
            execute_hist(num);
        }else if (strcmp(inpt, "myhistory -c") == 0){
            cmds_Num = 0;
            printf("History cleared\n");
        }else{
            add_cmd(inpt);
            system(inpt);
        }
    }

    return 0;
}

// Interactive Mode
void interactive_mode() {
    char line[MAX_LINE];

    while (1) {
        printf("newshell> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break; // Handle Ctrl-D (EOF)
        }

        // Remove newline and ignore empty lines
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        execute_command(line);
    }
}

// Batch Mode
void batch_mode(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline and ignore empty lines
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

        printf("%s\n", line); // Echo the command
        execute_command(line);
    }

    fclose(file);
}

// Execute Command
void execute_command(char *command) {
    char *args[MAX_LINE / 2 + 1];
    char *token = strtok(command, " ");
    int i = 0;

    int input_redirect = -1; // File descriptor for input redirection
    int output_redirect = -1; // File descriptor for output redirection

    while (token && i < MAX_LINE / 2) {
        if (strcmp(token, "<") == 0) { // Input redirection
            token = strtok(NULL, " ");
            input_redirect = open(token, O_RDONLY);
            if (input_redirect < 0) {
                perror("open for input");
                return;
            }
        } else if (strcmp(token, ">") == 0) { // Output redirection
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

    if (args[0] == NULL) return; // Ignore empty commands

    // Check for built-in commands
    if (strcmp(args[0], "cd") == 0) {
        handle_cd(args[1]);
    } else if (strcmp(args[0], "exit") == 0) {
        handle_exit();
    } else {
        // Non-built-in commands: Use fork and exec
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            // Child process
            setpgid(0, 0); // Create a new process group for the child
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
            // Parent process
            setpgid(pid, pid);              // Set process group for the child
            tcsetpgrp(STDIN_FILENO, pid);   // Give terminal control to child
            waitpid(pid, NULL, WUNTRACED);  // Wait for the child process
            tcsetpgrp(STDIN_FILENO, getpid()); // Restore terminal to parent
        }
    }

    // Close file descriptors if opened
    if (input_redirect != -1) close(input_redirect);
    if (output_redirect != -1) close(output_redirect);
}

// Built-in Command: cd
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

// Built-in Command: exit
void handle_exit() {
    printf("Exiting shell...\n");
    exit(0);
}

// Built-in Command: path
void handle_path(char *args) {
    handle_alias(args);
}

// Built-in Command: myhistory
void add_cmd(char *cmd) {
    // TODO: Implement history functionality
}
