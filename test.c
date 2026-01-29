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

int main() {
    int c = 0;
    while (1) {
        c++;                                       // increase utime
        // printf("%d\n", c);
        // sleep(5);
        int status;
        waitpid(-1, &status, WNOHANG); // system call to increase stime
    }
    return 0;
}