#include "file.h"

/*
 * get_size returns the size of the file in bytes.
 */
int get_size(FILE* fp) {
    
    if (!fp) {
        return -1;
    }
    
    if (fseek(fp, 0, SEEK_END) < 0) {
        return -1;
    }
    
    int len = ftell(fp);
    
    if (len < 0) {
        return -1;
    } else {
        rewind(fp);
        return len;
    }
}
