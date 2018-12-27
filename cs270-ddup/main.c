/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */

#include "error.h"
#include "trace.h"
#include "utility.h"
#include "files.h"

#include <stdlib.h>
#include <stdio.h>

void program_main(int argc, char *argv[] );

int main(int argc, char *argv[] ){
    e_wwrap_err_checking(program_main, argc, argv);
    return 1;
}

void program_main(int argc, char *argv[] ){
    u_fill_mode(argc, argv);
    f_copy_src_dest(argv[argc-2],argv[argc-1]);
}
