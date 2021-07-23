#ifndef EXECUTEHEADER_H
#define EXECUTEHEADER_H

#include "stack.h"
#include "parser.h"
#include "operations.h"
#include <stdlib.h>

/* 
 * execute begins executing the current instruction 
 * (the instruction the program counter is upto).
 */
void execute(struct stack* s, struct program* p);

/*
 * overflow checks for stack overflow conditions, and exits accordingly.
 */
void overflow(struct stack* s, struct program* p);

/*
 * error_check checks if the current state of the program is or will
 * reach an error state in the next instruction call.
 */
void error_check(struct stack* s, struct program* p);

#endif
