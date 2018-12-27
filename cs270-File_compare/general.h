#ifndef GENERAL_H
#define GENERAL_H
/**
 * General.h
 * ---------
 * contains functionality to compute the hamming distance,
 * print the size
 * and invert the file contents
 */

/**
 * g_ham_dist
 * ----------
 * computes the hamming distance between every byte in two arrays
 * and prints it, else, if the arrays are diffrent sizes, it prints a error
 * Args:
 *      char array, array size (twice)
 */
void g_ham_dist(const unsigned char * array_1, int size_1, const unsigned char * array_2, int size_2);

/**
 * g_invert
 * --------
 * inverts the bytes in an array and prints it to std out
 * Args:
 *      char array, array size
 */
void g_invert(const unsigned char * array, int size);

/**
 * g_size
 * ------
 * Prints the size you pass it, I really don't know why
 * I made this a function
 * Args:
 *      Size to print
 */
void g_size(int size);
  #endif