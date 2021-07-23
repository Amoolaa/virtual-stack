#include "vm_x2017.h"

int main(int argc, char* argv[]) {

    /* If invalid arguments, exit program. */
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments.\n");
        return 1;
    }

    /* Opening file. b specifier is for compatibility with other systems. 
     * If an error occurs, exit program. 
     */
    FILE* fp = fopen(*(argv + 1), "rb");
    if (!fp) {
        fprintf(stderr, "File error.\n");
        return 1;
    }
    
    int len = get_size(fp);
    
    /* If get_size returns -1, an error has occurred. */
    if (len == -1) {
        fprintf(stderr, "File error.\n");
        fclose(fp);
        return 1;
    
    /* Program should exit when empty binaries are given. */
    } else if (len == 0) {
        fprintf(stderr, "Empty binary file.\n");
        fclose(fp);
        return 1;
    } else if (len > MAX_BYTES) {
        fprintf(stderr, "Binary file is too large.\n");
        fclose(fp);
        return 1;
    }
    
    /* Contains decimal representation of hexdump */
    BYTE dec[MAX_BYTES];
    
    /* We read into the buffer, and check for any unexpected errors. */
    if (fread(dec, sizeof(BYTE), len, fp) < sizeof(BYTE) * len) {
        
        if (ferror(fp) != 0) {
            fprintf(stderr, "fread has encountered a serious error.\n");
            return 1;
        }
        
        if (feof(fp)) {
            fprintf(stderr, "Unexpected EOF encountered.\n");
            return 1;
        }
    }
        
    fclose(fp);    

    /* program contains all information to do with the program code. 
     * The binary information is parsed into the relevant fields
     * in the program struct. 
     */
    struct program p;
    p.num_functions = 0;
    parser(dec, len, &p);
    
    /* stack contains all information to do with the state of the program
     * during execution.
     */
    struct stack s;
    init_stack(&s);
    
    /* Loop until we have reached the end of the program or stack overflow. */
    while (stack_manager(&s, &p)) {
        execute(&s, &p);
    }

	return 0;
}
