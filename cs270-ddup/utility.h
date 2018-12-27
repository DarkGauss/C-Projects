/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */

#ifndef UTILITY_H
#define UTILITY_H

typedef enum{
    Force,
    Verbose,
    Backup,
    Confirm
} u_mode_t;


/**
 * u_fill_mode
 * -----------
 *  Files g_modes variable with modes specficed
 *  which can be checked by u_chk_mode('mode');
 *  in char**, modes begin with -'Char' if an invalid 
 *  mode is passed, it raises an error and terminates
 *  Returns: none
 *  Input: same as main
 */
void u_fill_mode(int, char**);

/**
 * u_chk_mode
 * ----------
 *  If mode passed by mode type is in g_mode
 *  then it returns 1, else false
 *  Input: mode type 
 *  returns: 1 if in, 0 if not
 */
int u_chk_mode(u_mode_t);


#endif
