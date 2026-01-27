#include <unistd.h>
#include <stdio.h>

int main() {
    int c = 0;
    while (1) {
        c++;
        printf("%d\n", c);
        sleep(5);
    }
    return 0;
}