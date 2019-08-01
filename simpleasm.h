//
// Created by Liam Williams on 2019-07-16.
//

#ifndef SIMPLEASM_SIMPLEASM_H
#define SIMPLEASM_SIMPLEASM_H

typedef struct {
    char *operation;
    unsigned int *address;
} operation;

typedef struct {
    char *label;
    unsigned int *address;
} label_definition;

int label_array_size_increase(size_t);

void usage(char *);

unsigned int get_opcode(operation *);

void run_opcodes(void);

unsigned int *add_label_placeholder(char*);

unsigned int *add_or_find_label_address(char*);

void add_operation(operation*);

void add_operation_label(char*, operation*);

void decode_opcode(unsigned short, unsigned short *, unsigned short *);

int allocate_operation_element(size_t);

void add_opcode(int, operation*);

void run_instruction(unsigned short, const unsigned short *, short *, unsigned short *);

// Lexer methods

extern int yylex(void);
extern void yyset_in(FILE*);

#endif //SIMPLEASM_SIMPLEASM_H
