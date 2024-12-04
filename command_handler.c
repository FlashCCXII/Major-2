#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024

void execute_command(char *command);
void handle_exit();
void handle_alias(char *alias, char *command);
void execute_alias(char *alias);

typedef struct {
	char alias[MAX_LINE];
	char command[MAX_LINE];
} Alias;

#define MAX_ALIASES 10
Alias aliases[MAX_ALIASES];
int alias_count = 0;

void execute_command(char *command) {
	char *args[MAX_LINE / 2 + 1];
	char *token = strtok(command, " ");
	int i = 0;

	while (token && i < MAX_LINE / 2) {
	args[i++] = token;
	token = strtok(NULL, " ");
	}
	args[i] = NULL;

	if (args[0][0] == '#') {
	execute_alias(args[0]);
	} else {
	pid_t pid = fork();
	if (pid == 0) {
	if (execvp(args[0], args) == -1) {
	perror("Error executing command");
	exit(1);
	}
	} else if (pid <0) {
	perror("fork failed");
	} else {
	wait(NULL);
	)
}
}

void handle_exit() {
	printf("Exiting the shell...\n");
	exit(0);
	}

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
		execute_command(aliases[i].command);
		return;
	}
}
	printf("Alias not found.\n");
}
