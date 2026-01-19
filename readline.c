#include <stdio.h>
#include <stdlib.h>

char * readline(void) {
    //from https://stackoverflow.com/a/314422
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;
    
    if (line == NULL)
        return NULL;
    
    while (1) {
        c = fgetc(stdin);
        if (c == EOF)
            break;
        
        if (--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if (linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }
        if ((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

int main() {
    while (1){
        printf(readline());
    }
    return 0;
}