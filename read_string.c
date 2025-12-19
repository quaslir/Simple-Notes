#include <stdio.h>
#include <stdlib.h>


void read_string(char ** buf) {
    int c;
    int count = 0;
    while((c = getchar()) != '\n' &&(c != EOF)) {
        char * tmp = realloc(*buf, (count+2) * sizeof(char));
        if(!tmp) {
            fprintf(stderr, "Memory reallocation failed\n");
            return;
        }
        *buf = tmp;
        (*buf)[count] = c;
        count++;
    }
    (*buf)[count] = '\0';
}