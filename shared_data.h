#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#define HISTORY_SIZE 20

extern char *history[HISTORY_SIZE];
extern int history_count;

void add_cmd(char *cmd);
void print_history();
void execute_history(int num);

#endif // SHARED_DATA_H
