#ifndef STACKHEADER_H
#define STACKHEADER_H

#include <stdlib.h>
#include "parser.h"
#include "getters.h"

#define MAX_RAM 256 /* Maximum allocated RAM */
#define PC 7        /* Program counter */
#define BP 6        /* Index of the start of the frame (base pointer) */
#define FL 5        /* Label of function being processed (Function Label) */

/* Returns address/index of stack variable denoted by symbol A */
#define GET_ADDRESS(s, A) (s)->registers[BP] + (A) + 2 

/* Accesses value of stack variable denoted by symbol A */
#define GET_STK(s, A) (s)->ram[GET_ADDRESS(s, A)]

/* Accesses value at register denoted by value A */
#define GET_REG(s, A) (s)->registers[(A)]

/* Accesses value at address/index held by stack symbol A */
#define GET_PTR(s, A) (s)->ram[GET_STK(s, A)]

struct stack {
    /* Information about execution is stored as a call stack in the RAM.
     * Each stack frame starts with the previous program counter of the
     * calling routine, the label of the calling routine and then space
     * allocated for stack variables (stack variables start from the
     * second index of the stack frame).
     * 
     * Each stack frame:
     * [..., return PC, previous function label, local variables, ...]
     */
    BYTE ram[MAX_RAM];
    
    /* registers[PC] = program counter
     * registers[BP] = base pointer 
     * registers[FL] = function label
     * Last personal register left unused for now
     */    
    BYTE registers[8];
};

/* 
 * init_stack initialises all relevant fields of the stack struct.
 */
void init_stack(struct stack* s);

/*
 * stack_manager takes the current state of the machine. If we have reached
 * the end of the program, returns 0. Otherwise, returns 1.
 */
int stack_manager(struct stack* s, struct program* p);

#endif
