#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

// Function declarations

void execute_command(char *command);
void handle_exit();
void handle_alias(char *alias, char *command);

#endif
