#include "alias_handler.h"
#include "command_handler.h"
#include "shared_data.h"
#include <stdio.h>
#include <string.h>

void handle_input(char *input) {
    if (strncmp(input, "alias ", 6) == 0) {
        char *alias = strtok(input + 6, "=");
        char *command = strtok(NULL, "\n");
        if (alias && command) {
            handle_alias(alias, command);
            printf("Alias '%s' set to '%s'\n", alias, command);
        } else {
            printf("Invalid alias syntax. Use: alias <name>=<command>\n");
        }
    } else if (strncmp(input, "history", 7) == 0) {
        print_history();
    } else if (strncmp(input, "!!", 2) == 0) {
        if (history_count > 0) {
            handle_input(history[history_count - 1]);
        } else {
            printf("No commands in history.\n");
        }
    } else {
        add_cmd(input);  
        execute_command(input);
	}
}
