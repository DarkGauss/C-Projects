/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */

#include "utility.h"
#include "error.h"
#include "trace.h"
//see header for definitions
void u_fill_mode(int argc, char** argv);
int u_chk_mode(u_mode_t input);

/**
 * u_set_modes
 * ----------
 *  Sets the mode specfied by the input
 *  as true in g_mode variable
 *  Input: mode to set
 *  Returns: nothing
 */
static void u_set_mode(u_mode_t input);

//stores the current modes of the program
static unsigned int u_modes;

void u_fill_mode(int argc, char** argv){
    IF_DEBUG t_push("u_fille_mode(%d, %p)", argc, argv);

    //loop counter
    int i;
    //initialize u_modes
    u_modes = 0;

    //in order to add new options, add case to switch
    //only checks first letter after -
    for(i = 0; argv[i] != '\0'; i++){
        if(argv[i][0] == '-'){
            switch(argv[i][1]){
                case 'F':
                    u_set_mode(Force);
                    break;
                case 'V':
                    u_set_mode(Verbose);
                    break;
                case 'b':
                    u_set_mode(Backup);
                    break;
                case 'c':
                    u_set_mode(Confirm);
                    break;
                default:
                    e_error(OPTIONS);
            }
        }
    }
    IF_DEBUG t_pop();
    return;
}


int u_chk_mode(u_mode_t input){
    IF_DEBUG t_push("u_chk_mode(%d)", input);
    //checks if postion offet by input is a 1
    IF_DEBUG t_pop();
    return u_modes & (1<< (int)input);
}


static void u_set_mode(u_mode_t input){
    IF_DEBUG t_push("u_set_mode(%d)", input);
    //mask contains a 1 and postion corosponding with mode
    unsigned int mask = (1 << (int)input);
    
    //flip location at mask
    u_modes ^= mask;
    IF_DEBUG t_pop();
    return;
}
