#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

void execute_command(char *command);
void handle_exit();
void handle_alias(char *alias, char *command);
void execute_alias(char *alias);

#endif // COMMAND_HANDLER_H
