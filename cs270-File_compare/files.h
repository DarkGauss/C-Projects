#ifndef FILES_H
#define FILES_H

#include <stdlib.h>
/**
 * files.h
 * -------
 * handles reading in of the files
 */

 /**
  * f_open_one
  * ----------
  * Handles opening of one files and stores the bytes in the passed destination
  * Args:
  *     Filename, location to store bytes, loc to store size
  */
void f_open_one(char * filename, unsigned char ** dest, size_t * size);

/**
 * f_open_two
 * ----------
 * Handles opening of two files and stores the bytes in the passed destination
 * Args:
 *     Filename, location to store bytes, loc to store size (twice)
 */
void f_open_two(char * filename_1, unsigned char ** dest_1, size_t * size_1,
                char * filename_2, unsigned char ** dest_2, size_t * size_2);

#endif