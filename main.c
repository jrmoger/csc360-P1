#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "linked_list.h"

#define MAXTOK 50
#define PATH_MAX 100

Node* head = NULL;

int parse(char *line, char *argv[], int max) {
    // parse terminal input as array of arguments

    int argc = 0;
    char *tok = strtok(line, " \t\n");
    while (tok && argc < max - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;
    return argc;
}

pid_t convert_pid(char* input) {
    // converts string to pid
    char *endptr;
    errno = 0;
    long x = strtol(input, &endptr, 10);
    if (errno != 0) {
        perror("strtol");
        return -1;
    }
    if (endptr == input || *endptr != '\0') {
        fprintf(stderr, "invalid pid: %s\n", input);
        return -1;
    }
    if (x < 0) {
        printf("invalid pid: %ld\n", x);
    }
    return (pid_t)x;
}

void func_BG(int argc, char *argv[]){
    if (argc < 2) {
        printf("usage: bg [process path]\n");
        return;
    }

    char path[PATH_MAX];
    snprintf(path, sizeof(path), "./%s", argv[1]); // add ./ to start of path
    printf("path: %s\n", path);
    
    //printf("argc: %d\nargv[0]: %s\nargv[1]: %s\n", argc, argv[0], argv[1]);
    char* args[argc];
    for (int i = 0; i < argc; i++) {
        args[i] = NULL;
    }
    // copy argv minus bg
    for (int i = 0; i < argc - 1; i++) {
        args[i] = argv[i+1];
        printf("args[%d]: %s\n", i, args[i]);
    }


    if (access(path, F_OK) != 0) {
        // check if path is valid
        printf("invalid program path\n");
        return;
    }

    // if (realpath(path, args)) {
    //     perror("realpath");
    //     exit(1);
    // }

    pid_t pid = fork();
    if (pid == 0) {
        // exec program if fork is child

        //char *args[]={argv[1], NULL};
        //char path[PATH_MAX];
        
        //char *args[] = {argv[1], path, NULL};
        execvp(path, args);
        perror("execvp");
        exit(1);
    } else {
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG); // check if child exists
        if (result == 0) {
            printf("pid child: %d\n", pid);
            head = add_newNode(head, pid, argv[1]);
        } else {
            printf("no child\n");
        }
        // if (PifExist(head, pid)) {
        //     head = add_newNode(head, pid, argv[1]);
        // } else {
        //     printf("pid %d already exists\n", (int)pid);
        // }
    }    
}


void func_BGlist(int argc, char* argv[]){
    printf("printing list:\n");
	printList(head);
}


void func_BGkill(int argc, char* argv[]){
    if (argc != 2) {
        printf("usage: bgkill <pid>\n");
        return;
    }

    pid_t pid = convert_pid(argv[1]);
    if (pid < 0) {
        // invalid pid
        return;
    }
    // printf("pid: %d\n", (int)pid);
    if (PifExist(head, pid) == 0) {
        printf("pid does not exist\n");
    }

    int val = kill(pid, SIGTERM);
    if (val == 0) {
        head = deleteNode(head, pid);
        printf("successfully killed process %d\n", pid);
    }
}


void func_BGstop(char * str_pid){
	//Your code here
    printf("func_BGstop\n");
}


void func_BGstart(char * str_pid){
	//Your code here
    printf("func_BGstart\n");
}


void func_pstat(int argc, char* argv[]){
	//Your code here
    printf("func_pstat\n");
    printList(head);
}

 
int main(){
    char line[50];
    while (1) {
        printf("Pman: > ");
        if (!fgets(line, sizeof(line), stdin)) break;

        char *argv[MAXTOK];
        int argc = parse(line, argv, MAXTOK);
        if (argc == 0) continue;

        if (strcmp(argv[0], "bg") == 0) {
            func_BG(argc, argv);
        } else if (strcmp(argv[0], "bglist") == 0) {
            func_BGlist(argc, argv);
        } else if (strcmp(argv[0], "bgkill") == 0) {
            func_BGkill(argc, argv);
        // } else if (strcmp(argv[0], "bgstop") == 0) {
        //     func_BGstop(argc, argv);
        // } else if (strcmp(argv[0], "bgstart") == 0) {
        //     func_BGstart(argc, argv);
        } else if (strcmp(argv[0], "pstat") == 0) {
            func_pstat(argc, argv);
        } else if (strcmp(argv[0], "q") == 0) {
            printf("bye bye\n");
            exit(0);
        } else {
            printf("Invalid input\n");
        }
    }


  return 0;
}

