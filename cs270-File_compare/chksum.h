#ifndef CHKSUM_H
#define CHKSUM_H
/** chksum.h
 * Calc's checksums and stuff, sleepy will ad more here
 */

 /**
  * c_parity_check
  *---------------
  * Xor's every byte together, and prints the result
  */
 void c_parity_check(const unsigned char * array, int size);

 /**
  * c_mod_sum
  *----------
  * Sums up each byte, mod MAX_CHAR (ie overflows), and then
  * takes the two's complement of the result and prints it.
  */
 void c_mod_sum(const unsigned char * array, int size);

#endif