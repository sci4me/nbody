#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "util.h"

char* read_file(char *name) {
    FILE *fp = fopen(name, "rb");
    if(!fp) {
        fprintf(stderr, "Couldn't load `%s`\n", name);
        exit(-1);
        return 0;
    }

    fseek(fp, 0L, SEEK_END);
    u32 size = ftell(fp);
    rewind(fp);

    char *code = malloc(size + 1);

    for(int i = 0; i < size; i++) {
        code[i] = fgetc(fp);
    }
    code[size] = 0;

    return code;
}

float rand01() {
    return (float) rand() / RAND_MAX;
}

float randm11() {
    return (float) (rand() - (RAND_MAX / 2)) / RAND_MAX;
}