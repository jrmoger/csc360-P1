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

#define MAXTOK 64
#define PATH_MAX 128
#define LINE_MAX 128
#define NAME_MAX 64

Node* head = NULL;

int parse(char *line, char *argv[], int max) {
    // parse terminal input as array of arguments
    // returns number of arguments

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
    // returns pid, or -1 if failed

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
    char fullpath[PATH_MAX];

    // check if path starts with / or .
    if (argv[1][0] == '/' || argv[1][0] == '.') {
        snprintf(path, sizeof(path), "%s", argv[1]);
    } else {
        snprintf(path, sizeof(path), "./%s", argv[1]); // add ./ to start of path
    }
    
    if (realpath(path, fullpath) == NULL) {            // get full path 
        // perror("realpath");
        printf("invalid program path\n");
        return;
    }
    // printf("path: %s\n", path);
    // printf("fullpath: %s\n", fullpath);

    char* args[argc];
    
    // copy argv minus bg
    for (int i = 0; i < argc - 1; i++) {
        args[i] = argv[i+1];
        // printf("args[%d]: %s\n", i, args[i]);
    }
    args[argc-1] = NULL;                               // end argument array with null
    
    // check if path is valid
    if (access(fullpath, F_OK) != 0) {
        printf("invalid program path\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // exec program if fork is child
        execvp(fullpath, args);
        perror("execvp");
        exit(1);
    } else {
        sleep(1);
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG); // check if child exists
        if (result == 0) {
            printf("child pid: %d\n", pid);
            head = add_newNode(head, pid, fullpath);
        } else {
            printf("status %d, child %d terminated\n", status, pid);
        }
    }
}

void func_BGlist(int argc, char* argv[]){
	printList(head);
    printf("total background jobs: %d\n", listSize(head));
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

    if (PifExist(head, pid) == 0) {
        printf("pid does not exist\n");
        return;
    }

    int val = kill(pid, SIGTERM);
    if (val == 0) {
        head = deleteNode(head, pid);
        printf("sent TERM signal to program %d\n", pid);
    } else {
        printf("%d error", val);
    }
}

void func_BGstop(int argc, char* argv[]){
	if (argc != 2) {
        printf("usage: bgstop <pid>\n");
        return;
    }
    pid_t pid = convert_pid(argv[1]);
    if (pid < 0) {
        // invalid pid
        return;
    }
    if (PifExist(head, pid) == 0) {
        printf("pid does not exist\n");
        return;
    }
    
    int val = kill(pid, SIGSTOP);
    if (val == 0) {
        printf("sent STOP signal to program %d\n", pid);
    } else {
        printf("%d error", val);
    }
}

void func_BGstart(int argc, char* argv[]){
	if (argc != 2) {
        printf("usage: bgstart <pid>\n");
        return;
    }
    pid_t pid = convert_pid(argv[1]);
    if (pid < 0) {
        // invalid pid
        return;
    }
    if (PifExist(head, pid) == 0) {
        printf("pid does not exist\n");
        return;
    }
    
    int val = kill(pid, SIGCONT);
    if (val == 0) {
        printf("sent CONT signal to program %d\n", pid);
    } else {
        printf("%d error", val);
    }
}

int read_stat(  
    pid_t pid, 
    char comm[NAME_MAX], 
    char *state, 
    unsigned long long *utime, 
    unsigned long long *stime, 
    long *rss
) {
    // reads pid, comm, state, utime, and stime from /proc/<pid>/stat
    // returns 0 on success, -1 if fail

    char path[PATH_MAX];
    snprintf(path, sizeof(path), "/proc/%d/stat", (int)pid);

    FILE *f = fopen(path, "r");
    if (!f) {
        return -1;
    } 

    int n = fscanf( 
        f,
        "%*s"                                          // 1 skip
        "%255s %c"                                     // 2 comm, 3 state
        "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s"      // 4-13 skip
        "%llu %llu"                                    // 14 utime, 15 stime
        "%*s %*s %*s %*s %*s %*s %*s %*s"              // 16-23 skip
        "%ld",                                         // 24 rss
        comm, state, utime, stime, rss
    );
    fclose(f);
    if (n == 5) {                                      // check if there are 5 values
        return 0;
    } else {
        return -1;
    }
}

int read_status(pid_t pid, long *voluntary, long *nonvoluntary) {
    // reads voluntary_ctxt_switches and nonvoluntary_ctxt_switches from proc/<pid>/stat
    // returns 0 on success, -1 on failure

    char path[PATH_MAX];
    snprintf(path, sizeof(path), "/proc/%d/status", (int)pid);

    FILE *f = fopen(path, "r");
    if (!f) {
        return -1;
    }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        // lines are like:
        // "voluntary_ctxt_switches:\t123\n"
        // "nonvoluntary_ctxt_switches:\t123\n"
        if (strncmp(line, "voluntary_ctxt_switches:", 24) == 0) {
            long v;
            if (sscanf(line + 24, "%ld", &v) == 1) *voluntary = v;
        } else if (strncmp(line, "nonvoluntary_ctxt_switches:", 27) == 0) {
            long nv;
            if (sscanf(line + 27, "%ld", &nv) == 1) *nonvoluntary = nv;
        }
    }
    fclose(f);
    if (voluntary < 0 || nonvoluntary < 0) return -1;
    return 0;
}

void func_pstat(int argc, char* argv[]){
	if (argc != 2) {
        printf("usage: pstat <pid>\n");
        return;
    }
    pid_t pid = convert_pid(argv[1]);
    if (pid < 0) {
        // invalid pid
        return;
    }
    if (kill(pid, 0) != 0) {                           // check if pid exists
        printf("pid does not exist\n");
        return;
    }

    char comm[NAME_MAX];
    char state;
    unsigned long long utime;
    unsigned long long stime;
    long rss;

    if (read_stat(pid, comm, &state, &utime, &stime, &rss) != 0) {
        perror("read_stat");
        return;
    }

    long v_ctxt;
    long nv_ctxt;

    if (read_status(pid, &v_ctxt, &nv_ctxt)) {
        perror("read_status");
        return;
    }

    printf("1. comm  : %s\n", comm);
    printf("2. state : %c\n", state);
    printf("3. utime : %Lf\n", utime / (long double)sysconf(_SC_CLK_TCK));
    printf("4. stime : %Lf\n", stime / (long double)sysconf(_SC_CLK_TCK));
    printf("5. rss   : %ld\n", rss);
    printf("6. voluntary_ctxt_switches    : %ld\n", v_ctxt);
    printf("7. nonvoluntary_ctxt_switches : %ld\n", nv_ctxt);

}
 
int main(){
    char line[LINE_MAX];
    while (1) {
        printf("Pman: > ");
        if (!fgets(line, sizeof(line), stdin)) {       // read terminal input        
            break;
        } 

        char *argv[MAXTOK];
        int argc = parse(line, argv, MAXTOK);
        if (argc == 0) continue;

        if (strcmp(argv[0], "bg") == 0) {
            func_BG(argc, argv);
        } else if (strcmp(argv[0], "bglist") == 0) {
            func_BGlist(argc, argv);
        } else if (strcmp(argv[0], "bgkill") == 0) {
            func_BGkill(argc, argv);
        } else if (strcmp(argv[0], "bgstop") == 0) {
            func_BGstop(argc, argv);
        } else if (strcmp(argv[0], "bgstart") == 0) {
            func_BGstart(argc, argv);
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