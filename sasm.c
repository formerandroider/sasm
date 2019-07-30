//
// Created by Liam Williams on 2019-07-30.
//

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "sasm.h"

void usage(char *);

int main(int argc, char **argv) {

    FILE *file = NULL;

    if (argc < 2) {
        usage(argv[0]);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "simpleasm: %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    set_file(file);
    do_parse();
    start();

    return 0;
}

void usage(char *pname) {
    printf("Usage: %s <file>\n", pname);
    exit(1);
}