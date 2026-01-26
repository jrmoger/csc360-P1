#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"

#define MAXTOK 50

Node* head = NULL;

int parse(char *line, char *argv[], int max) {
    // parse terminal input

    int argc = 0;
    char *tok = strtok(line, " \t\n");
    while (tok && argc < max - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;   // argv must be NULL-terminated
    return argc;
}

void func_BG(int argc, char *argv[]){
    if (argc < 2) {
        printf("usage: bg [process name]\n");
    }
    printf("argc: %d\nargv[0]: %s\nargv[1]: %s\n", argc, argv[0], argv[1]);
    // printf("cmd: %s\n", path);
    // if (access(path, F_OK) != 0) {
    //     // file does not exist
    //     printf("invalid program name\n");
    //     return;
    // }
    // pid_t pid = fork();
    // if (pid == 0) {
    //     execl(path, cmd[1], (char *)NULL);
    // } else {
    //     if (PifExist(head, pid)) {
    //         head = add_newNode(head, pid, cmd[1]);
    //     } else {
    //         printf("pid %d already exists\n", (int)pid);
    //     }
    // }
}


void func_BGlist(char **cmd){
	//Your code here;
    printf("func_BGlist\n");
}


void func_BGkill(char * str_pid){
	//Your code here
    printf("func_BGkill\n");
}


void func_BGstop(char * str_pid){
	//Your code here
    printf("func_BGstop\n");
}


void func_BGstart(char * str_pid){
	//Your code here
    printf("func_BGstart\n");
}


void func_pstat(char * str_pid){
	//Your code here
    printf("func_pstat\n");
    printList(head);
}

 
int main(){
    // char user_input_str[50];
    // while (1) {
    //     printf("Pman: > ");
    //     fgets(user_input_str, 50, stdin);
    //     printf("User input: %s \n", user_input_str);
    //     char * ptr = strtok(user_input_str, " \n");
    //     if(ptr == NULL){
    //         continue;
    //     }
    //     char * lst[50];
    //     int index = 0;
    //     lst[index] = ptr;
    //     index++;
    //     while(ptr != NULL){
    //         ptr = strtok(NULL, " \n");
    //         lst[index]=ptr;
    //         index++;
    //     }
    //     if (strcmp("bg",lst[0]) == 0){
    //         func_BG(lst);
    //     } else if (strcmp("bglist",lst[0]) == 0) {
    //         func_BGlist(lst);
    //     } else if (strcmp("bgkill",lst[0]) == 0) {
    //         func_BGkill(lst[1]);
    //     } else if (strcmp("bgstop",lst[0]) == 0) {
    //         func_BGstop(lst[1]);
    //     } else if (strcmp("bgstart",lst[0]) == 0) {
    //         func_BGstart(lst[1]);
    //     } else if (strcmp("pstat",lst[0]) == 0) {
    //         func_pstat(lst[1]);
    //     } else if (strcmp("q",lst[0]) == 0) {
    //         printf("Bye Bye \n");
    //         exit(0);
    //     } else {
    //         printf("Invalid input\n");
    //     }
    // }
    char line[50];

    while (1) {
        printf("Pman: > ");
        if (!fgets(line, sizeof(line), stdin)) break;

        char *argv[MAXTOK];
        int argc = parse(line, argv, MAXTOK);
        if (argc == 0) continue;

        if (strcmp(argv[0], "bg") == 0) {
            func_BG(argc, argv);
        // } else if (strcmp(argv[0], "bglist") == 0) {
        //     func_BGlist(argc, argv);
        // } else if (strcmp(argv[0], "bgkill") == 0) {
        //     func_BGkill(argc, argv);
        } else if (strcmp(argv[0], "q") == 0) {
            printf("bye bye\n");
            exit(0);
        } else {
            printf("Invalid input\n");
        }
    }


  return 0;
}

