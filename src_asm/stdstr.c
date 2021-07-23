#include "stdstr.h"

/*
 * str_cpy copies the string src to dest. Does not copy the null byte.
 */
void str_cpy(char* dest, char* src) {
    while (*(src) != '\0')
        *(dest++) = *(src++);
}
