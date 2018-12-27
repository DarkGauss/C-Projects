#include "error.h"
#include "trace.h"
#include "utility.h"
#include "files.h"
#include "general.h"
#include "freq.h"
#include "runs.h"
#include "chksum.h"

#include <stdlib.h>
#include <stdio.h>

void program_main(int argc, char *argv[] );

int main(int argc, char *argv[] )
{
    e_wwrap_err_checking(program_main, argc, argv);
    return 1;
}

void program_main(int argc, char *argv[] )
{
    u_fill_mode(argc, argv);

    //store raw bytes of the file/s
    unsigned char * file_1 = NULL;
    unsigned char * file_2 = NULL;
    size_t file_1_size;
    size_t file_2_size;
    
    //fill up the arrays with bytes from the file/s
    if(u_chk_mode(M_Hamming))
    {
        //make sure the user supplied two files
        if(argv[argc-2][0] == '-') e_error(ARGUMENTS);
        f_open_two(argv[argc-2], &file_1, &file_1_size,argv[argc-1], &file_2, &file_2_size);
    } 
    else 
    {
        f_open_one(argv[argc-1], &file_1, &file_1_size);
    }

    //execute the code for each option
    if(u_chk_mode(M_Hamming))
    {
        g_ham_dist(file_1, file_1_size, file_2, file_2_size);
    }
    if(u_chk_mode(M_Inverse))
    {
        g_invert(file_1,file_1_size);
    }
    if(u_chk_mode(M_Size))
    {
        g_size(file_1_size);
    }
    if(u_chk_mode(M_Freq))
    {
        fr_freq_unsorted(file_1,file_1_size);
    }
    if(u_chk_mode(M_Freq_sorted))
    {
        fr_freq_sorted(file_1,file_1_size);
    }
    if(u_chk_mode(M_Run_longest))
    {
        r_run_longest(file_1, file_1_size);
    }
    if(u_chk_mode(M_Run_all))
    {
        r_run_all(file_1, file_1_size);
    }
    if(u_chk_mode(M_Chksum_par))
    {
        c_parity_check(file_1, file_1_size);
    }
    if(u_chk_mode(M_Chksum_mod))
    {
        c_mod_sum(file_1, file_1_size);
    }
    free(file_1);
    if(file_2 != NULL) free(file_2);
}

