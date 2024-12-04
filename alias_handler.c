#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_handler.c"

#define MAX_LINE 1024

void handle_input(char *input) {
	if (strncmp(input, "alias ", 6) == 0) {
	char *alias = strtok(input + 6, "=");
	char *command = strtok(NULL, "\n");
	if (alias && command) {
	handle_alias(alias, command);
	printf("Alias %s set to %s\n", alias, command);
	} else {
	printf("Invalid alias syntax. Use: alias <name>=<command>\n");
	}
	} else {
	execute_command(input);
	}
	}
int main() {
	char input[MAX_LINE];

	while (1) {
	printf("shell> ");
	if (fgets(input, MAX_LINE, stdin) != NULL) {
	input[strlen(input) - 1] = '\0';
	if (strcmp(input, "exit") == 0) {
	handle_exit();
	} else {
	handle_input(input);
	}
	} else {
	printf("Error reading input\n");
	}
	}
	}
