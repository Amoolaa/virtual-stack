#ifndef OPERATIONSHEADER_H
#define OPERATIONSHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "parser.h"
#include "getters.h"

/*
 * move copies the value of the second argument to a memory type and location
 * denoted by the first argument.
 */
void move(struct stack* s, struct instruction i);

/*
 * call modifies the program counter, current function, base pointer and
 * creates a new stack frame such that the program will execute the function 
 * specified in the first argument of CAL.
 */
void call(struct stack* s, struct instruction i, struct function f);

/*
 * ret returns from the current function and proceeds executing the calling
 * function. The program counter and base pointer are restored to 
 * their previous values, and the current function label is updated.
 */
void ret(struct stack* s, struct program* p);

/*
 * ref takes the address (in this implementation, the index of the RAM array) 
 * of the second argument (stack symbol) and places it in the first argument. 
 */
void ref(struct stack* s, struct instruction i);

/*
 * add takes two register addresses and adds their values, storing
 * the result in the first register address given as an argument.
 */
void add(struct stack* s, struct instruction i);

/*
 * print takes any address/value type and prints the contents as an unsigned
 * integer to a new line of stdout.
 */
void print(struct stack* s, struct instruction i);

/*
 * not takes a register address and performs bitwise NOT operation (~),
 * storing the result in the register.
 */
void not(struct stack* s, struct instruction i);

/*
 * equal takes a register address and checks if it's value is zero. If the
 * value is zero, the register stores 1, or otherwise stores 0. 
 */
void equal(struct stack* s, struct instruction i);

#endif
