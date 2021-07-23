#ifndef PARSERFILE_H
#define PARSERFILE_H

#include <stdio.h>
#include <stdlib.h>
#include "bin.h"

/* Number of bits used to represent the following elements in the hexdump */
#define INSTR_COUNT_BIT_SIZE 5
#define OPCODE_BIT_SIZE 3
#define FUNC_ID_BIT_SIZE 3
#define TYPE_BIT_SIZE 2
#define VAL_BIT_SIZE 8
#define REG_BIT_SIZE 3
#define STK_BIT_SIZE 5
#define PTR_BIT_SIZE 5

/* If 3 bits represent function id, there can be 8 functions */
#define MAX_FUNCTIONS 8

/* If 5 bits represent instruction count, there can be 31 instructions,
 * since having 0 instructions is invalid.
 */
#define MAX_INSTRUCTIONS 31

/* If 5 bits represent stack symbols, there can be 32 symbols */
#define MAX_STACK_SYMBOLS 32

/* Largest instruction can be 20 bits (e.g MOV STK A VAL B) */
#define MAX_INSTRUCTION_WIDTH 20

/* Each function can be 3 + 5 + 31*20 = 628 bits long. If there are 8 functions
 * the binary can be a max of 648 bytes long. This accounts for when RET does
 * not appear at the end of functions, so that it can throw appropriate errors.
 */
#define MAX_BYTES 628

/*
 * opcodes contains all associated decimal values to the opcodes 
 * used in instructions.
 */
enum opcodes {
    MOV,
    CAL,
    RET,
    REF,
    ADD,
    PRINT,
    NOT,
    EQU
};

/*
 * types contains all associated decimal values to the types
 * used in instructions.
 */
enum types {
    VAL,
    REG,
    STK,
    PTR
};

/*
 * instruction contains all relevant information (opcode, arguments) 
 * needed to print and execute the particular instruction.
 */
struct instruction {
    int opcode;
    int first_type;
    int first_val;
    int second_type;
    int second_val;
};

/*
 * function contains all relevant instruction and symbol information
 * needed to print and execute the particular function.
 */
struct function {
    int id;
    int num_instructions;
    struct instruction instructions[MAX_INSTRUCTIONS];
    int num_symbols;
    
    /* Holds symbols in order of appearance in the function. */
    BYTE symbol_table[MAX_STACK_SYMBOLS]; 
};

/*
 * program contains all the program code, parsed into function and instruction
 * structs which contain all the fields necessary to print and execute the
 * program.
 */
struct program {
    int num_functions;
    struct function functions[MAX_FUNCTIONS];
};


/*
 * init_instr initialises all fields of an instruction struct.
 */
void init_instr(struct instruction* i, int opcode, int first_type, 
                            int first_val, int second_type, int second_val);

/*
 * parser takes in the binary representation of the hexdump and parses
 * it (in reverse). Data is stored in program code structs (program,
 * function, instruction) to be used later in printing and execution. 
 */
void parser(BYTE* dec, int len, struct program* p);

/*
 * get_val_size returns the size (in bits) of the value associated with
 * the given type.
 */
int get_val_size(int type);

/*
 * symbol_exists returns the index of the symbol in the symbol table if it
 * exists, and returns -1 otherwise. 
 */
void init_symbol_table(struct function* f);

/*
 * symbol_exists returns the index of the symbol in the symbol table if it
 * exists, and returns -1 otherwise. 
 */
int symbol_exists(BYTE s, BYTE symbol_table[], int len);

/* 
 * init_symbols changes STK and PTR symbols to byte offsets on the stack.
 * These are used during execution.
 */
void init_symbols(struct function* f);

#endif
