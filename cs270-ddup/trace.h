/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */

#ifndef TRACE_H
#define TRACE_H

#include <stdarg.h>

/**
 * e_setup_trace
 * -------------
 *  Setups up the functionality for stack tracing
 *  by allocating memory to the s_trace_stack array
 *  for initially 20 deep
 * Args: none
 * Return: none
 */
void t_setup_trace();

/**
 * t_push
 * ---------
 *  Pushes a function to the trace stack for printing
 *  if an error eccors or for tehlulz. Aruments passed are
 *  the same as printf()
 * Usage:
 *      CALL AT THE BEGINING OF FUNCTION, guard with if(debug)...
 * Args: Pass the function name and its args
 *        ie: t_push_f("myfunc(%d, %d)",arg1,arg2)
 * Returns: None
 */
void t_push(const char *format, ...);

/**
 * t_pop
 * -------
 *  Pops a function off the trace stack
 * Usage:
 *      CALL AT THE END OF YOUR FUNCTION, plz
 * Args: none
 * Return: none
 */
void t_pop();

/**
 * t_print_trace
 * -------------
 *  Prints the trace of functions at the point this function is called to stderr
 * 
 * Args: none
 * Retunrs: none
 */
void t_print_trace();

#endif
