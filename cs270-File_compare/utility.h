#ifndef UTILITY_H
#define UTILITY_H

typedef enum{
    M_Freq,         //-f lists frequecies
    M_Freq_sorted, //-F  lists frequencies sorted
    M_Hamming,     //-H Computers hamming dist
    M_Inverse,     //-I Prints inverse
    M_Run_longest, //-r Prints longest run
    M_Run_all,     //-R Prints all runs
    M_Size,        //-s Prints file size
    M_Chksum_par,  //-x prints longitudial parity check
    M_Chksum_mod  //-X Prints moduler sumation
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