#ifndef RUNS_H
#define RUNS_H
/**
 * runs.h
 * ------
 * Calculates the run length encoded runs in an array
 */

 /**
  * r_longest_run
  * -------------
  * Calculates the longest run in a array
  * Args:
  *   char array, size
  */
void r_run_longest(const unsigned char * array, int size);

/**
 * r_run_all
 * ---------
 * Calculates and displays all runs in a given array
 * Args:
 *   char array, size
 */
void r_run_all(const unsigned char * array, int size);

#endif