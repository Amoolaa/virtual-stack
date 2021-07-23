#ifndef GETTERSHEADER_H
#define GETTERSHEADER_H

#include "parser.h"

/* Returns the function struct currently being executed */
#define GET_CURRENT_FUNC(s, p) \
        (p)->functions[get_func_index((p), GET_REG(s, FL))]

/* Returns the instruction struct currently being executed */
#define GET_CURRENT_INSTR(s, p) GET_CURRENT_FUNC(s, p).\
        instructions[GET_CURRENT_FUNC(s, p).num_instructions - GET_REG(s, PC)]

/*
 * get_func_index returns the index of the function with specified id within
 * the array of functions stored in the program code struct.
 */
int get_func_index(struct program* p, int id);

#endif
