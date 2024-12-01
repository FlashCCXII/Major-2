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
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [batchFile]\n", argv[0]);
        exit(1);
    }

    if (argc == 2) {
        batch_mode(argv[1]);
    } else {
        interactive_mode();
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
    // Tokenize the input command
    char *args[MAX_LINE / 2 + 1];
    char *token = strtok(command, " ");
    int i = 0;

    while (token && i < MAX_LINE / 2) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Check for built-in commands
    if (strcmp(args[0], "cd") == 0) {
        handle_cd(args[1]); // Pass the second argument to handle_cd} 
        else if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);} 
        else {
        // Non-built-in commands: Use fork and exec
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");} 
        else if (pid == 0) {
            // Child process
            if (execvp(args[0], args) < 0) {
                perror("execvp");
                exit(1);
            }
        } 
        else {
            // Parent process waits
            wait(NULL);
        }
    }
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
    // TODO: Implement path manipulation
}

// Built-in Command: myhistory
void handle_myhistory(char *args) {
    // TODO: Implement history functionality
}
