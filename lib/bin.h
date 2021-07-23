#ifndef BINARYHEADER_H
#define BINARYHEADER_H

#define BYTE unsigned char
#define BITS 8      /* number of bits in a byte */

/*
 * get_bits uses bitwise operations to return the decimal value of n bits from
 * the starting bit 'start' of an array 'dec' of 8-bit integers.
 */
int get_bits(BYTE* dec, unsigned int len, unsigned int start, BYTE n);

#endif
