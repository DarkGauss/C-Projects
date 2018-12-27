#ifndef FREQ_H
#define FREQ_H
/**
 * Calculates the frequences of bytes within arrays and prints them
 */

 /**
  * fr_freq_unsorted
  * ----------------
  * prints a unorted frequency list of bytes
  * in the input array
  * Args:
  *     unsigned char array, size
  */
  void fr_freq_unsorted(const unsigned char * array, int size);

  /**
   * fr_freq_sorted
   * --------------
   * displays a decending sorted list
   * of the frequency for bytes in file
   * Args:
   *    unsigned char array, size
   */
  void fr_freq_sorted(const unsigned char * array, int size);

#endif