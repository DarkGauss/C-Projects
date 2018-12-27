/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */
#ifndef ERROR_H
#define ERROR_H

//used to control wheter traciny is enabled
#define DEBUG 1
#define IF_DEBUG if(DEBUG)

//defines macro to store line and file number
#define e_error( error_code ) e_error_lf(error_code, __FILE__, __LINE__)
#define e_error_t( error_code , error_text) e_error_lft(error_code, __FILE__, __LINE__, error_text)
/**
 * Type varible for supported error codes
 * used to call error handing functions
 */
typedef enum{
    NO_ERROR,
    ERROR,
    ERROR_TEXT,
    OPTIONS,
    SYSTEM_CALL,
    MALLOC
} e_error_t;

/**
 *  e_nwrap_err_checking, e_wwrap_err_checking
 * ------------------------------
 *  Wraps the function passed with functionality
 *  for catching (kinda) handling errors
 * 
 * args: the function to wrap or function with std args
 * returns: void
 */

void e_nwrap_err_checking(void (*func)());
void e_wwrap_err_checking(void (*func)(int, char **), int, char**);



/**
 * e_error_lf
 * --------
 *  Handles errorcode passed and terminates the program
 *  with line and filename listed
 * 
 * Args: Error code to handle, char* to file, int linenumber
 * Returns: hahahah it never will ;)
 */
void e_error_lf(e_error_t, char *, int);

/**
 * e_error_lft
 * --------
 *  Handles errorcode passed and terminates the program
 *  with line and filename listed, also stores error text supplied
 *  which will be printed with error
 * 
 * Args: Error code to handle, char* to file, int linenumber
 * Returns: hahahah it never will ;)
 */
void e_error_lft(e_error_t, char *, int, char*);

#endif
