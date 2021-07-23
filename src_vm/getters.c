#include "getters.h"

/*
 * get_func_index returns the index of the function with specified id within
 * the array of functions stored in the program code struct.
 */
int get_func_index(struct program* p, int id) {
    int i;
    int n = p->num_functions;
    for (i = 0; i < n; i++) {
        if (p->functions[i].id == id) {
            return i;
        }
    } 
    return -1;
}
