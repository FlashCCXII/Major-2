#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "alias_handler.h"
#include "command_handler.h"
#include "new_history.h" 

// Constants
#define MAX_LINE 1024
#define HISTORY_SIZE 20

// Global Variables
char *history[HISTORY_SIZE];
int history_count = 0;
char *path = NULL;

// Function Prototypes
void interactive_mode();
void batch_mode(const char *filename);
void handle_path(char *args);
void handle_exit();

int main(int argc, char *argv[]) {
    signal(SIGINT, SIG_IGN);   // Ignore Ctrl-C
    signal(SIGTSTP, SIG_IGN);  // Ignore Ctrl-Z

    // Initialize the path variable with the current PATH
    path = strdup(getenv("PATH"));
    if (!path) {
        perror("strdup");
        exit(1);
    }
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

        handle_input(line); // Handle the command
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
        line[strcspn(line, "\n")] = 0; // Remove newline
        if (strlen(line) == 0) continue;

        handle_input(line); // Handle the command
    }

    fclose(file);
}

void handle_path(char *args) {
    if (args == NULL) {
        // Print current PATH
        printf("Current PATH: %s\n", path);
    } else if (args[0] == '+') {
        // Add to PATH
        if (strstr(path, args + 1)) {
            printf("Directory '%s' is already in PATH.\n", args + 1);
            return;
        }
        // Append the new directory to PATH
        char *new_path = (char *)malloc(strlen(path) + strlen(args) + 2);
        if (!new_path) {
            perror("malloc");
            return;
        }
        sprintf(new_path, "%s:%s", path, args + 1);
        free(path);
        path = new_path;
        printf("Updated PATH: %s\n", path);
    } else if (args[0] == '-') {
        // Remove from PATH (implement accordingly)
    } else {
        fprintf(stderr, "Invalid argument for path command.\n");
    }
}

void handle_exit() {
    printf("Exiting shell...\n");
    free(path);
    exit(0);
}
