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
            handle_input(aliases[i].command);
            return;
        }
    }
    printf("Alias not found.\n");
}
