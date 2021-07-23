#include "execute.h"

/*
 * error_check checks if the current state of the program is or will
 * reach an error state in the next instruction call.
 */
void error_check(struct stack* s, struct program* p) {
    
    /* Checking if the current function label exists as a function */
    if (get_func_index(p, GET_REG(s, FL)) == -1) {
        fprintf(stderr, "Function %d does not exist.\n", GET_REG(s, FL));
        exit(1);
    
    /* Checking if PC is out of bounds */
    } else if (GET_REG(s, PC) > GET_CURRENT_FUNC(s, p).num_instructions) {
        fprintf(stderr, "Program counter out of bounds. Instruction %d does "
                        "not exist for function %d.\n", 
                        GET_REG(s, PC), GET_REG(s, FL));
        exit(1);
    }
}

/* 
 * execute begins executing the current instruction 
 * (the instruction the program counter is upto).
 */
void execute(struct stack* s, struct program* p) {
    
    /* Error checking before executing next function. */
    error_check(s, p);
    overflow(s, p);
    
    /* Program counter is incremented before executing instruction */
    GET_REG(s, PC)++;
    
    /* Instruction associated with opcode is executed. */
    switch (GET_CURRENT_INSTR(s, p).opcode) {

        case PRINT:
            print(s, GET_CURRENT_INSTR(s, p));
            break;
        
        case MOV:
            move(s, GET_CURRENT_INSTR(s, p));
            break;
        
        case ADD:
            add(s, GET_CURRENT_INSTR(s, p));
            break;
        
        case EQU:
            equal(s, GET_CURRENT_INSTR(s, p));
            break;
        
        case NOT:
            not(s, GET_CURRENT_INSTR(s, p));
            break;
        
        case CAL:
            call(s, GET_CURRENT_INSTR(s, p), GET_CURRENT_FUNC(s, p));
            break;
        
        case RET:
            ret(s, p);
            break;
        
        case REF:
            ref(s, GET_CURRENT_INSTR(s, p));
            break;
    }
}

/*
 * overflow checks for stack overflow conditions, and exits accordingly.
 */
void overflow(struct stack* s, struct program* p) {
    /* If the current stack frame exceeds the maximum RAM, the program
     * should exit immediately due to stack overflow.
     */
    if (GET_REG(s,BP) + GET_CURRENT_FUNC(s, p).num_symbols > MAX_RAM - 1) {
        fprintf(stderr, "Stack Overflow.\n");
        exit(1);
    }
}
