#include "parser.h"

/*
 * parser takes in the binary representation of the hexdump and parses
 * it (in reverse). Data is stored in program code structs (program,
 * function, instruction) to be used later in printing and execution. 
 */
void parser(BYTE* dec, int len, struct program *p) {
    
    int first_type = 0;
    int first_val = 0;
    int second_type = 0;
    int second_val = 0;   
    
    /* We use bit_start to represent the starting bit in the range
     * of bits read. This is updated before any get_bit request. It
     * starts at the last bit, and we decrement to find the new
     * starting bit before every get_bit request.
     */
    int bit_start = len * BITS + 1;
    
    /* We loop until all bits have been processed, i.e. until we have reached 
     * the padding bits.
     */ 
    int i, j;
    for (i = 0; i < MAX_FUNCTIONS && bit_start > BITS; i++) { 
        p->num_functions++;
        
        /* We use bit_start to represent the starting bit in the range
         * of bits read. This is updated before any get_bit request.
         */

        /* First 5 bits in a function represent the number of instructions */
        bit_start -= INSTR_COUNT_BIT_SIZE;
        BYTE n = get_bits(dec, len, bit_start, INSTR_COUNT_BIT_SIZE);
        
        /* Parsing through each instruction. */
        for (j = 0; j < n; j++) {
            
            struct instruction* current = (p->functions[i].instructions) + j;
             
            /* opcode is the first 3 bits in an instruction */
            bit_start -= OPCODE_BIT_SIZE;
            BYTE opcode = get_bits(dec, len, bit_start, OPCODE_BIT_SIZE); 

            /* For each opcode, we parse through the required number of bits
             * and store the information data in its struct. In cases where
             * the opcode does not take all two arguments, -1 is placed in the
             * relevant field.
             */
            switch (opcode) {
                
                case MOV:
                case REF:
                case ADD:
                    bit_start -= TYPE_BIT_SIZE;
                    first_type = get_bits(dec, len, bit_start, TYPE_BIT_SIZE);
                    
                    bit_start -= get_val_size(first_type);
                    first_val = get_bits(dec, len, bit_start, 
                                                    get_val_size(first_type));
                    
                    bit_start -= TYPE_BIT_SIZE;
                    second_type = get_bits(dec, len, bit_start, TYPE_BIT_SIZE);
                    
                    bit_start -= get_val_size(second_type);
                    second_val = get_bits(dec, len, bit_start, 
                                                    get_val_size(second_type));
                    
                    init_instr(current, opcode, first_type, first_val, 
                                                    second_type, second_val);
                    break;
                
                case CAL:
                case PRINT:
                case NOT:
                case EQU:
                    bit_start -= TYPE_BIT_SIZE;
                    first_type = get_bits(dec, len, bit_start, TYPE_BIT_SIZE);

                    bit_start -= get_val_size(first_type);
                    first_val = get_bits(dec, len, bit_start, 
                                                    get_val_size(first_type));

                    init_instr(current, opcode, first_type, first_val, -1, -1);
                    break;
                
                case RET:
                    init_instr(current, opcode, -1, -1, -1, -1);
                    break;
            }
        }
        /* Last 3 bits in a function represent the function label */
        bit_start -= FUNC_ID_BIT_SIZE;
        BYTE func_id = get_bits(dec, len, bit_start, FUNC_ID_BIT_SIZE);

        p->functions[i].id = func_id;
        p->functions[i].num_instructions = n;
        
        /* As per the spec, the last instruction in any function must be RET.
         * Since we parse in reverse, the first instruction in the array is
         * the last, and should be RET. Otherwise, exit the program.
         */       
        if (p->functions[i].instructions[0].opcode != RET) {
            fprintf(stderr, "Function %d must terminate with RET.\n", func_id);
            exit(1);
        }    
    
        /* The symbol table and symbols associated with instructions are also
         * loaded into each function.
         */
        init_symbol_table(&(p->functions[i]));
        init_symbols(&(p->functions[i]));
    }    
}

/*
 * symbol_exists returns the index of the symbol in the symbol table if it
 * exists, and returns -1 otherwise. 
 */
int symbol_exists(BYTE s, BYTE symbol_table[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (symbol_table[i] == s) {
            return i;
        }
    }
    return -1;
}

/* 
 * init_symbols changes STK and PTR symbols to byte offsets on the stack.
 * These are used during execution.
 */
void init_symbols(struct function* f) {
    int i;
    int n = f->num_instructions;

    /* Looping through each instruction, and reconfiguring STK and
     * PTR values.
     */
    for (i = 0; i < n; i++) {
        struct instruction* c = &(f->instructions[n - i - 1]);
        if (c->first_type == STK || c->first_type == PTR) {
            c->first_val = symbol_exists(c->first_val, f->symbol_table, 
                                                            f->num_symbols);
        }
        if (c->second_type == STK || c->second_type == PTR) {
            c->second_val = symbol_exists(c->second_val, f->symbol_table, 
                                                            f->num_symbols);
        }
    }
}

/*
 * init_symbol_table adds stack symbols to an array in order of appearance
 * in the function.
 */
void init_symbol_table(struct function* f) {
    int i;
    int index = 0;
    int n = f->num_instructions;
   
    /* Looping through each instruction and adding them to the symbol table. */
    for (i = 0; i < n; i++) {
        /* Accessing the current instruction */
        struct instruction c = f->instructions[n - i - 1];
        
        if ((c.first_type == STK || c.first_type == PTR) &&
                symbol_exists(c.first_val, f->symbol_table, index) == -1) {
            
            f->symbol_table[index] = c.first_val;
            index++;
        }
        
        if ((c.second_type == STK || c.second_type == PTR) &&
                symbol_exists(c.second_val, f->symbol_table, index) == -1) {
            
            f->symbol_table[index] = c.second_val;
            index++;
        }
    }
    
    f->num_symbols = index;
}


/*
 * init_instr initialises all fields of an instruction struct.
 */
void init_instr(struct instruction* i, int opcode, int first_type, 
                            int first_val, int second_type, int second_val) {
    i->opcode = opcode;
    i->first_type = first_type;
    i->first_val = first_val;
    i->second_type = second_type;
    i->second_val = second_val;
}

/*
 * get_val_size returns the size (in bits) of the value associated with
 * the given type.
 */
int get_val_size(int type) {
    switch (type) {
        case VAL:
            return VAL_BIT_SIZE;
            break;
        case REG:
            return REG_BIT_SIZE;
            break;
        case STK:
            return STK_BIT_SIZE;
            break;
        case PTR:
            return PTR_BIT_SIZE;
            break;
        default:
            return -1;
            break;
    }
}

