#include "alias_handler.h"
#include "command_handler.h"
#include "new_history.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024

typedef struct {
    char alias[MAX_LINE];
    char command[MAX_LINE];
} Alias;

#define MAX_ALIASES 20  // Increase the alias limit if needed
Alias aliases[MAX_ALIASES];
int alias_count = 0;

void handle_alias(char *alias, char *command) {
    if (alias_count < MAX_ALIASES) {
        // Safely copy alias and command using snprintf
        snprintf(aliases[alias_count].alias, MAX_LINE, "%s", alias);
        snprintf(aliases[alias_count].command, MAX_LINE, "%s", command);
        alias_count++;
    } else {
        printf("Alias limit reached.\n");
    }
}

int is_alias(char *cmd) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].alias, cmd) == 0) {
            return 1; // Alias found
        }
    }
    return 0; // Alias not found
}

void execute_alias(char *alias) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].alias, alias) == 0) {
            printf("Executing alias: %s\n", aliases[i].command);
            // Instead of calling handle_input directly, we return the command to execute
            // This way, the main command handler can decide what to do with it
            handle_input(aliases[i].command);
            return;
        }
    }
    printf("Alias not found.\n");
}

// Additional function to list all aliases
void list_aliases() {
    if (alias_count == 0) {
        printf("No aliases defined.\n");
        return;
    }

    printf("Defined aliases:\n");
    for (int i = 0; i < alias_count; i++) {
        printf("%s -> %s\n", aliases[i].alias, aliases[i].command);
    }
}

// Additional function to delete an alias
void delete_alias(char *alias) {
    int found = 0;
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].alias, alias) == 0) {
            found = 1;
            // Shift all subsequent aliases down by one
            for (int j = i; j < alias_count - 1; j++) {
                aliases[j] = aliases[j + 1];
            }
            alias_count--;
            printf("Alias '%s' deleted.\n", alias);
            break;
        }
    }
    if (!found) {
        printf("Alias '%s' not found.\n", alias);
    }
}

