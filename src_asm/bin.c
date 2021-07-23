#include "bin.h"

/*
 * get_bits uses bitwise operations to return the decimal value of n bits from
 * the starting bit 'start' of an array 'dec' of 8-bit integers.
 */
int get_bits(BYTE* dec, unsigned int len, unsigned int start, BYTE n) {
    
    /* If start is greater than the number of bits in the decimal array, then
     * function should exit. Additionally, for this implementation, we only
     * need to read 8 bits.
     */
    if (start + n - 1 > len * BITS || n == 0 || n > 8) {
        return -1;
    }

    /* The starting and ending bytes are the indexes of the decimal array that
     * the starting bit and ending bit are located.
     */
    int start_byte = (start - 1) / BITS;
    int end = start + n - 1;
    int end_byte = (end - 1) / BITS;
    
    BYTE val = 0;
    
    /* If the starting and ending bits belong in the same byte, we perform a
     * simple bitmask.
     */
    if (start_byte == end_byte) {
        /* pos is the position of the starting bit from start of the byte. */
        BYTE pos = BITS - start % BITS;

        /* We shift the required bits to the start of the byte, and then
         * use a bitmask to get the value.
         */ 
        val = (dec[start_byte] >> (pos + 1 - n)) & ~(~0 << n);
    
    /* If the starting and ending bits belong in consecutive bytes, then
     * we separate into two bit operations - the end and starting sections
     * of each byte. These sections are divided by the 8 bit limit for bytes,
     * i.e. 00011[011|1101]0000, where we wish to retrieve the 7 bits in the
     * middle.
     */
    } else {
        
        /* n_end_bits is the number of bits in the ending section. */
        BYTE n_end_bits = end % BITS;
        
        /* pos_end is the position of the ending bit from start of the byte */
        BYTE pos_end = BITS - end % BITS;
        
        /* We calculate the ending section in the same way as done above. */
        val += (dec[end_byte] >> pos_end) & ~(~0 << n_end_bits);
        
        /* We calculate the starting section by noticing that we have 
         * n - n_end_bits to determine, so we find its decimal value. We then
         * shift by the number of end bits to account for the starting sections
         * position of bits.
         */
        val += (dec[start_byte] & ~(~0 << (n - n_end_bits))) << (n_end_bits);
    }
    
    return val;
}
