//
// Created by Liam Williams on 2019-07-29.
//

#ifndef SIMPLEASM_SASM_H
#define SIMPLEASM_SASM_H

#include <stdio.h>

typedef struct {
    char *operation;
    unsigned int *address;
} operation;

typedef struct {
    char *label;
    unsigned int *address;
} label_definition;

void set_file(FILE*);
void do_parse(void);
void start(void);

#endif //SIMPLEASM_SASM_H
