#ifndef PRINTHEADER_H
#define PRINTHEADER_H

#include <stdio.h>
#include "parser.h"
#include "stdstr.h"
#include "bin.h"

/* Instruction line can be 21 chars in length (e.g ____MOV_STK_A_VAL_255).
 * This does not include the null byte or newlines.
 */
#define MAX_LINE_LEN 21

/* Values are from 0-255, so a value can have 3 digits. */
#define MAX_VAL_DIGITS 3

/*
 * print_asm prints all the program code associated with a .x2017 file
 * to standard output.
 */
void print_asm(struct program p);

/* 
 * print_instr prints each instruction line by building an 
 * instruction string with relevant opcodes, arguments.
 */
void print_instr(struct instruction i);

/* 
 * opcode_to_str converts opcode to string form, storing in buffer.
 * Returns the number of characters copied.
 */
int opcode_to_str(int opcode, char* buf);

/* 
 * type_to_str converts type to string form, storing in buffer.
 * Returns the number of characters copied.
 */
int type_to_str(int type, char* buf);

/*
 * symbol_to_char converts the value associated with a stack symbol to
 * the corresponding character.
 */
char symbol_to_char(BYTE s);

#endif
