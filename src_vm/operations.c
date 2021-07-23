#include "operations.h"

/*
 * move copies the value of the second argument to a memory type and location
 * denoted by the first argument.
 */
void move(struct stack* s, struct instruction i) {
   
    /* The first argument cannot be value typed; so, we cannot have VAL as
     * the first type. We check for every other type.
     */
    if (i.first_type == REG) {
        switch (i.second_type) {
            
            case REG:
                GET_REG(s, i.first_val) = GET_REG(s, i.second_val);
                break;
            
            case VAL:
                GET_REG(s, i.first_val) = i.second_val;
                break;
            
            case STK:
                GET_REG(s, i.first_val) = GET_STK(s, i.second_val);
                break;
            
            case PTR:
                GET_REG(s, i.first_val) = GET_PTR(s, i.second_val);
                break;
        }
    } else if (i.first_type == STK) {
        switch (i.second_type) {
            
            case REG:
                GET_STK(s, i.first_val) = GET_REG(s, i.second_val);
                break;
            
            case VAL:
                GET_STK(s, i.first_val) = i.second_val;
                break;
            
            case STK:
                GET_STK(s, i.first_val) = GET_STK(s, i.second_val);
                break;
            
            case PTR:
                GET_STK(s, i.first_val) = GET_PTR(s, i.second_val);
                break;
        }
    } else if (i.first_type == PTR) {
        switch(i.second_type) {
            
            case REG:
                GET_PTR(s, i.first_val) = GET_REG(s, i.second_val);
                break;
            
            case VAL:
                GET_PTR(s, i.first_val) = i.second_val;
                break;
            
            case STK:
                GET_PTR(s, i.first_val) = GET_STK(s, i.second_val);
                break;
            
            case PTR:
                GET_PTR(s, i.first_val) = GET_PTR(s, i.second_val);
                break;
        }

    /* Otherwise, exit the program, since the destination cannot be VAL. */
    } else {
        fprintf(stderr, "MOV: destination cannot be value typed.\n");
        exit(1);
    }
}

/*
 * call modifies the program counter, current function, base pointer and
 * creates a new stack frame such that the program will execute the function 
 * specified in the first argument of CAL.
 */
void call(struct stack* s, struct instruction i, struct function f) {
    
    /* First argument of call must be value typed. */
    if (i.first_type != VAL) {
        fprintf(stderr, "CAL: first argument must be VAL type.\n");
        exit(1);
    }
    
    /* Base pointer shifted upwards to start of new stack frame */
    GET_REG(s, BP) += f.num_symbols + 2;
    
    /* First item in stack frame becomes old program counter */
    s->ram[s->registers[BP]] = GET_REG(s, PC);
    
    /* Second item becomes the old function label */
    s->ram[s->registers[BP] + 1] = f.id; 
    
    /* Update function label to the new function called in the instruction */
    GET_REG(s, FL) = i.first_val;
    
    /* Update program counter to start of new function */
    GET_REG(s, PC) = 0;
}

/*
 * ret returns from the current function and proceeds executing the calling
 * function. The program counter and base pointer are restored to 
 * their previous values, and the current function label is updated.
 */
void ret(struct stack* s, struct program* p) {
    
    /* First item in stack frame contains the program counter of the calling
     * function (where the calling function left off). This becomes the new 
     * program counter, since we return execution to the calling function.
     */
    GET_REG(s, PC) = s->ram[s->registers[BP]];
    
    /* Second item in stack frame contains the label of the calling function.
     * We update the current function label to the calling function.
     */
    GET_REG(s, FL) = s->ram[s->registers[BP] + 1];

    /* Base pointer is shifted back to the start of the calling functions stack
     * frame.
     */
    GET_REG(s, BP) -= p->functions[get_func_index(p, GET_REG(s, FL))]
                                                            .num_symbols + 2;
}

/*
 * ref takes the address (in this implementation, the index of the RAM array) 
 * of the second argument (stack symbol) and places it in the first argument. 
 */
void ref(struct stack* s, struct instruction i) {
    
    /* Both arguments of REF cannot be values. */
    if (i.first_type == VAL || i.second_type == VAL) {
        fprintf(stderr, "REF: arguments cannot be VAL typed.\n");
        exit(1);
    }
    
    /* We cannot take the address of a register. */
    if (i.second_type == REG) {
        fprintf(stderr, "REF: second argument must be a stack symbol.\n");
        exit(1);   
    }

    if (i.second_type == STK) {
        switch (i.first_type) {
            case STK:
                GET_STK(s, i.first_val) = GET_ADDRESS(s, i.second_val);
                break;
            
            case REG:
                GET_REG(s, i.first_val) = GET_ADDRESS(s, i.second_val);
                break;
            
            case PTR:
                GET_PTR(s, i.first_val) = GET_ADDRESS(s, i.second_val);
                break;
        }
    
    /* The address of PTR A becomes &(*A), which is just the value of A. */
    } else if (i.second_type == PTR) {
        switch (i.first_type) {
            
            case STK:
                GET_STK(s, i.first_val) = GET_STK(s, i.second_val);
                break;
            
            case REG:
                GET_REG(s, i.first_val) = GET_STK(s, i.second_val);
                break;
            
            case PTR:
                GET_PTR(s, i.first_val) = GET_STK(s, i.second_val);
                break;
        }
    }
}

/*
 * add takes two register addresses and adds their values, storing
 * the result in the first register address given as an argument.
 */
void add(struct stack* s, struct instruction i) {
    
    /* ADD's arguments must be register typed */
    if (i.first_type != REG || i.second_type != REG) {
        fprintf(stderr, "ADD: both arguments must be registers.\n");
        exit(1);    
    }
    
    GET_REG(s, i.first_val) += GET_REG(s, i.second_val) ;
}

/*
 * print takes any address/value type and prints the contents as an unsigned
 * integer to a new line of stdout.
 */
void print(struct stack* s, struct instruction i) {
    switch (i.first_type) {
        
        case VAL:
            printf("%u\n", i.first_val);
            break;
        
        case REG:
            printf("%u\n", GET_REG(s, i.first_val));
            break;
        
        case STK:
            printf("%u\n", GET_STK(s, i.first_val));
            break;
        
        case PTR:
            printf("%u\n", GET_PTR(s, i.first_val));
            break;
    }
}

/*
 * not takes a register address and performs bitwise NOT operation (~),
 * storing the result in the register.
 */
void not(struct stack* s, struct instruction i) {
    
    /* NOT must have register argument. */
    if (i.first_type != REG) {
        fprintf(stderr, "NOT: argument must be register.\n");
        exit(1);
    }
    
    GET_REG(s, i.first_val) = ~GET_REG(s, i.first_val);
}

/*
 * equal takes a register address and checks if it's value is zero. If the
 * value is zero, the register stores 1, or otherwise stores 0. 
 */
void equal(struct stack* s, struct instruction i) {
    
    /* EQUAL must have register argument. */
    if (i.first_type != REG) {
        fprintf(stderr, "EQU: argument must be register.\n");
        exit(1);
    } 
    
    /* If register holds value of 0, store 1, and otherwise store 0. */
    if (GET_REG(s, i.first_val) == 0) {
        GET_REG(s, i.first_val) = 1;
    } else {
        GET_REG(s, i.first_val) = 0;
    }
}
