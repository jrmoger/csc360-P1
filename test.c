#include <unistd.h>

int main() {
    int c = 0;
    while (1) {
        c++;
        printf("%d\n", c);
        sleep(1);
    }
    return 0;
}