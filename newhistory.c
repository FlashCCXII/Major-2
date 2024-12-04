#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "new_history.h"
#include "command_handler.h"
#include "alias_handler.h"

// Maximum commands stored
#define HIST_MAX_CMDS 20

// Array to store commands
static char history[HIST_MAX_CMDS][1024];

// Number of commands stored in history
static int cmds_Num = 0;

// Add command to history
void add_cmd(const char *cmd) {
    // If history is full, shift all commands left by 1
    if (cmds_Num == HIST_MAX_CMDS) {
        for (int i = 0; i < HIST_MAX_CMDS - 1; i++) {
            strcpy(history[i], history[i + 1]);
        }
        cmds_Num--;
    }
    strncpy(history[cmds_Num], cmd, 1023);
    history[cmds_Num][1023] = '\0'; // Ensure null-termination
    cmds_Num++;
}

// Print history
void print_history() {
    for (int i = 0; i < cmds_Num; i++) {
        printf("%d: %s\n", i, history[i]);
    }
}

// Execute a command from history
void execute_history_command(int num) {
    if (num < 0 || num >= cmds_Num) {
        printf("Invalid command number\n");
        return;
    }

    char cmd[1024];
    strcpy(cmd, history[num]);
    printf("Executing command: %s\n", cmd);

    // Ensure command execution is compatible with your shell
    execute_command(cmd);
}

// Clear history
void clear_history() {
    cmds_Num = 0;
    printf("History cleared\n");
}

int main(){
    char inpt[1024];
    while (1){
        printf("> ");
        fgets(inpt, 1024, stdin);
        inpt[strlen(inpt)-1] = '\0';
        if(strcmp(inpt, "myhistory") == 0){
            print_hist();
        }else if (strncmp(inpt, "myhistory -e", 13) == 0){
            int num = atoi(inpt+13);
            execute_hist(num);
        }else if (strcmp(inpt, "myhistory -c") == 0){
            cmds_Num = 0;
            printf("History cleared\n");
        }else{
            add_cmd(inpt);
            system(inpt);
        }
    }
    return 0;
}
