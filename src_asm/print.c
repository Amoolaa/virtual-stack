#include "print.h"

/*
 * print_asm prints all the program code associated with a .x2017 file
 * to standard output.
 */
void print_asm(struct program p) {
    
    int i, j;
    for (i = 0; i < p.num_functions; i++) {
        
        /* Since we parsed binary in reverse, the functions are also in reverse
         * order. So, to print functions in the correct order, we start
         * at the end of the function array.
         */
        struct function current_func = p.functions[p.num_functions - i - 1];
        printf("FUNC LABEL %d\n", current_func.id);
        
        int n = current_func.num_instructions;
        for (j = 0; j < n; j++) {
            
            /* Similarly with functions, we start processing instructions at
             * the end of the instruction array.
             */
            struct instruction current_instr = current_func.
                                                    instructions[n - j - 1];
            
            print_instr(current_instr);
        }
    }
}

/*
 * symbol_to_char converts the value associated with a stack symbol to
 * the corresponding character.
 */
char symbol_to_char(BYTE s) {
    
    if (s < 26) {
        return s + 'A';
    } else {
        return s + 'A' + ('a' - 'Z' - 1);
    }
}

/* 
 * print_instr prints each instruction line by building an 
 * instruction string with relevant opcodes, arguments.
 */
void print_instr(struct instruction i) {
    
    /* Array used to build instruction string. */
    char instr_line[MAX_LINE_LEN + 1] = {0};
    char* ptr = instr_line;
    
    /* Spaces at beginning of instruction string are added */
    str_cpy(ptr, "    ");
    ptr += 4;
    
    /* Opcode is added to the instruction string */
    ptr += opcode_to_str(i.opcode, ptr);    
    
    /* If instruction takes no arguments, print and exit. */
    if (i.first_type == -1) {
        printf("%s\n", instr_line);
        return;
    }
    
    /* Adding first argument type to instruction string */
    *(ptr++) = ' ';
    ptr += type_to_str(i.first_type, ptr);
    *(ptr++) = ' ';
    
    /* If first type is a symbol, add the associated character. Otherwise,
     * add the decimal value (as a string) to the instruction string. 
     */
    if (i.first_type == STK || i.first_type == PTR) {
        *(ptr++) = symbol_to_char(i.first_val);
    } else {
        
        /* Converting decimal to string */
        char buf[MAX_VAL_DIGITS + 1];
        int digits = snprintf(buf, sizeof(buf), "%d", i.first_val);
        
        str_cpy(ptr, buf);
        ptr += digits;
    }
    
    /* If instruction takes only one argument, print and exit. */
    if (i.second_type == -1) {
        printf("%s\n", instr_line);
        return;
    }
    
    /* Adding second argument type to instruction string */
    *(ptr++) = ' ';
    ptr += type_to_str(i.second_type, ptr);
    *(ptr++) = ' ';
        
    /* If second type is a symbol, add the associated character. Otherwise,
     * add the decimal value (as a string) to the instruction string. 
     */
    if (i.second_type == STK || i.second_type == PTR) {
        *(ptr++) = symbol_to_char(i.second_val);
    } else { 
        
        /* Converting decimal to string */
        char buf[MAX_VAL_DIGITS + 1];
        int digits = snprintf(buf, sizeof(buf), "%d", i.second_val);
        
        str_cpy(ptr, buf);
        ptr += digits;
    }
    
    printf("%s\n", instr_line);
}

/* 
 * opcode_to_str converts opcode to string form, storing in buffer.
 * Returns the number of characters copied.
 */
int opcode_to_str(int opcode, char* buf) {
    switch (opcode) {
        
        case MOV:
            str_cpy(buf, "MOV");
            break;
        
        case CAL:
            str_cpy(buf, "CAL");
            break;
        
        case RET:
            str_cpy(buf, "RET");
            break;
        
        case REF:
            str_cpy(buf, "REF");
            break;
        
        case ADD:
            str_cpy(buf, "ADD");
            break;
        
        case PRINT:
            str_cpy(buf, "PRINT");
            return 5;
        
        case NOT:
            str_cpy(buf, "NOT");
            break;
        
        case EQU:
            str_cpy(buf, "EQU");
            break;
    }
    
    return 3;
}

/* 
 * type_to_str converts type to string form, storing in buffer.
 * Returns the number of characters copied.
 */
int type_to_str(int type, char* buf) {
    switch (type) {
        
        case VAL:
            str_cpy(buf, "VAL");
            break;
        
        case REG:
            str_cpy(buf, "REG");
            break;
        
        case STK:
            str_cpy(buf, "STK");
            break;
        
        case PTR:
            str_cpy(buf, "PTR");
            break;
    }
    
    return 3;
}
