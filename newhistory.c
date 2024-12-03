#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//maximum commands stored
#define HIST_MAX_CMDS 20
//array to store commands
char history[HIST_MAX_CMDS][1024];
//number of commands stored in hist
int cmds_Num = 0; 

//add command to history
void add_cmd(char *cmd){
    //if history is full, shift all cmds left by 1
    if (cmds_Num == HIST_MAX_CMDS){
        for(int i = 0; i < HIST_MAX_CMDS-1; i++){
            strcpy(history[i], history[i+1]);
        }
        cmds_Num--;
    }
    strcpy(history[cmds_Num], cmd);
    cmds_Num++;
}

//print history
void print_hist(){
    for(int i = 0; i < cmds_Num; i++){
        printf("%d: %s\n", i, history[i]);
    }
}

//executes cmd from history
void execute_hist(int num){
    if(num < 0 || num >= cmds_Num){
        printf("Invalid comand number\n");
        return;
    }
    char cmd[1024];
    strcpy(cmd, history[num]);
    printf("Executing command: %s\n", cmd);
    system(cmd);
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