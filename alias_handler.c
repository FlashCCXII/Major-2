#include "alias_handler.h"
#include "command_handler.h"
#include "new_history.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void handle_input(char *input) {
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
        execute_history_command(num);
    } else {
        printf("Invalid myhistory usage\n");
    }
    return;
    }
}
