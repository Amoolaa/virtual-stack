#include "stack.h"

/* 
 * init_stack initialises all relevant fields of the stack struct.
 */
void init_stack(struct stack* s) {
    
    GET_REG(s, PC) = 0;
    GET_REG(s, BP) = 0;
    
    /* Entry point is function with label 0. */
    GET_REG(s, FL) = 0; 
}

/*
 * stack_manager takes the current state of the machine. If we have reached 
 * the end of the program, returns 0. Otherwise, returns 1.
 */
int stack_manager(struct stack* s, struct program* p) {   
    
    /* If entry point doesn't exist, program should exit immediately. */
    if (get_func_index(p, GET_REG(s, FL)) == -1) {
        fprintf(stderr, "Entry point (FUNC LABEL 0) does not exist.\n");
        exit(1);
    }
      
    /* If we have reached the last instruction of the main function, 
     * execution should exit. This is a temporary variable and only
     * used for readability.
     */
    int at_last_instr = (GET_REG(s, PC) == 
                    p->functions[get_func_index(p, 0)].num_instructions - 1);
    
    
    if (at_last_instr && GET_REG(s, FL) == 0) {
        return 0;
    } else {
        return 1;
    }
}
