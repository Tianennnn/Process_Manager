#include <stdio.h>
#include <stdlib.h>

void *emalloc(size_t n) {
    void *p = NULL;
    p = malloc(n);
    if (p == NULL) {
        printf("Failed to malloc.");
        exit(-1);
    }
    return p;
}