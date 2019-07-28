//
// Created by Liam Williams on 2019-07-16.
//

#include <stdio.h>
#include <stdlib.h>
#include "simpleasm.h"
#include "lex.yy.h"

const unsigned short INST_HLT = 0x00;
const unsigned short INST_LDA = 0x01;
const unsigned short INST_JPA = 0x02;
const unsigned short INST_JPZ = 0x03;
const unsigned short INST_JPP = 0x04;
const unsigned short INST_ADD = 0x05;
const unsigned short INST_SUB = 0x06;
const unsigned short INST_STO = 0x07;
const unsigned short INST_OUT = 0x08;
const unsigned short INST_INP = 0x09;

unsigned short label_count = 0;
label_definition **label_definitions;

unsigned short next_address = 0;
short memory[100];

unsigned short operation_count = 0;
operation **operations;

extern void yyparse(void);

int main(int argc, char **argv) {

    FILE *file = NULL;
    char *pname = argv[0];

    if (argc < 2) {
        usage(pname);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "simpleasm: %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    yyset_in(file);
    yyparse();

    for (int i = 0; i < operation_count; i++) {
        add_opcode(i, operations[i]);
    }

    run_opcodes();

    return 0;
}

void decode_opcode(unsigned short opcode, unsigned short *operation, unsigned short *address) {
    *operation = opcode >> 7;
    *address = opcode & 0b0001111111;
}

void add_opcode(int address, operation *op) {
    if (op->address != NULL && *op->address == 100) {
        fprintf(stderr, "Syntax error, unreferenced label for operation %s\n", op->operation);
        exit(1);
    }

    memory[address] = get_opcode(op) << 7 | (op->address == NULL ? 0 : *op->address);
}

void add_operation(operation *op) {
    if (!allocate_operation_element(1)) {
        fprintf(stderr, "Unable to allocate memory.");
        abort();
    }

    operations[next_address++] = op;
    operation_count++;
}

void add_operation_label(char *label, operation *op) {
    int operation_address = next_address;

    unsigned int *label_addr = add_or_find_label_address(label);
    memcpy(label_addr, &operation_address, sizeof(operation_address));

    add_operation(op);
}

unsigned int *add_or_find_label_address(char *label) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(label_definitions[i]->label, label) == 0) {
            return label_definitions[i]->address;
        }
    }

    return add_label_placeholder(label);
}

unsigned int *add_label_placeholder(char *label) {
    label_definition *def = malloc(sizeof(label_definition));
    def->label = malloc(sizeof(char) * (strlen(label) + 1));
    strncpy(def->label, label, strlen(label) + 1);
    def->address = malloc(sizeof(unsigned int));
    memset(def->address, 100, sizeof(unsigned int));

    label_array_size_increase(1);
    label_definitions[label_count++] = def;

    return def->address;
}

void run_instruction(unsigned short instruction, const unsigned short *address, short *accumulator,
                     unsigned short *program_counter) {

    unsigned short increment_program_counter = 1;

    char *input = malloc(sizeof(char) * 100);
    switch (instruction) {
        case INST_ADD:
            *accumulator = *accumulator + memory[*address];
            break;
        case INST_OUT:
            printf("%d\n", *accumulator);
            fflush(stdout);
            break;
        case INST_SUB:
            *accumulator = *accumulator - memory[*address];
            break;
        case INST_STO:
            memory[*address] = *accumulator;
            break;
        case INST_HLT:
            exit(0);
            break;
        case INST_JPP:
            if (*accumulator < 0) {
                break;
            }
        case INST_JPZ:
            if (*accumulator != 0) {
                break;
            }
        case INST_JPA:
            *program_counter = *address;
            increment_program_counter = 0;
            break;
        case INST_LDA:
            *accumulator = memory[*address];
            break;
        case INST_INP:
            printf("Input requested: ");
            fflush(stdout);
            input = fgets(input, 100, stdin);
            // remove the \n character
            if (input[strlen(input) - 1] == '\n') {
                input[strlen(input) - 1] = '\0';
            }
            *accumulator = (short) strtol(input, NULL, 10);
            break;
        default:
            break;
    }

    if (increment_program_counter) {
        (*program_counter)++;
    }
}

void run_opcodes() {
    short accumulator = 0;
    unsigned short program_counter = 0;
    unsigned short instruction = 0;
    unsigned short address = 0;

    do {
        unsigned short opcode = memory[program_counter];
        decode_opcode(opcode, &instruction, &address);
        run_instruction(instruction, &address, &accumulator, &program_counter);
    } while (instruction != INST_HLT);
}

unsigned int get_opcode(operation *operation1) {
    char *op = operation1->operation;

    if (strcmp(op, "LDA") == 0) {
        return INST_LDA;
    } else if (strcmp(op, "JPP") == 0) {
        return INST_JPP;
    } else if (strcmp(op, "JPZ") == 0) {
        return INST_JPZ;
    } else if (strcmp(op, "JPA") == 0) {
        return INST_JPA;
    } else if (strcmp(op, "ADD") == 0) {
        return INST_ADD;
    } else if (strcmp(op, "SUB") == 0) {
        return INST_SUB;
    } else if (strcmp(op, "STO") == 0) {
        return INST_STO;
    } else if (strcmp(op, "HLT") == 0) {
        return INST_HLT;
    } else if (strcmp(op, "OUT") == 0) {
        return INST_OUT;
    } else if (strcmp(op, "DAT") == 0) {
        return 0;
    } else if (strcmp(op, "INP") == 0) {
        return INST_INP;
    } else {
        fprintf(stderr, "Unknown instruction encountered. Exiting.");
        abort();
    }

    return 0;
}

void usage(char *pname) {
    printf("Usage: %s <file>\n", pname);
    exit(1);
}

int label_array_size_increase(size_t count) {
    if (label_definitions == NULL) {
        label_definitions = malloc(sizeof(label_definition *) * count);
    } else {
        label_definitions = realloc(label_definitions, sizeof(label_definition *) * (label_count + count));
    }

    if (!label_definitions) {
        return 0;
    }

    memset(label_definitions + label_count, 0, count);

    return 1;
}

int allocate_operation_element(size_t count) {
    if (operations == NULL) {
        operations = malloc(sizeof(operation *) * count);
    } else {
        operations = realloc(operations, sizeof(operation *) * (operation_count + count));
    }

    if (operations == NULL) {
        return 0;
    }

    memset(operations + operation_count, 0, count);

    return 1;
}
