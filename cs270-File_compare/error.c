#include "error.h"
#include "trace.h"

#include <setjmp.h>
#include <stdio.h>
#include <errno.h>

//see header for deffinitions
void e_nwrap_err_checking(void (*func)());
void e_wwrap_err_checking(void (*func)(int, char **),int argc, char ** argv);
void e_error_lf(e_error_t error_code, char * file, int line);
void e_error_lft(e_error_t error_code, char * file, int line, char* error_text);

/**
 * e_error_nlf
 * --------
 *  Handles errorcode passed and terminates the program
 *  without printing line and file
 * 
 * Args: Error code to handle
 * Returns: hahahah it never will ;)
 */
void e_error_nlf(e_error_t error_code);

/**
 * e_handle_error
 * --------------
 *  Handles error provided by error_code
 * Args: Error code
 * Returns: void
 */
static void e_handle_error(e_error_t error_code);

/**
 * Functions to hanndle error code that ends their name
 */
static void e_handle_system_call();


//Stores global jmpback point for error checking
static jmp_buf entry_state;

//stores which file error occured
static char * e_file;

//stores line number of error
static int e_line;

//stores error text supplied
static char * e_error_text;

//stores wheter error text has been supplied
static int e_error_text_value;

void e_nwrap_err_checking(void (*func)()){
    //stores error_code returned
    e_error_t error_code;
    e_error_text_value = 0;

    IF_DEBUG t_setup_trace();
    if(( error_code = setjmp(entry_state) )){
        e_handle_error(error_code);
        return;
    }
    (*func)();
    return;
}

void e_wwrap_err_checking(void (*func)(int, char **), int argc, char** argv){
    //stores error_code returned
    e_error_t error_code;
    e_error_text_value = 0;

     IF_DEBUG t_setup_trace();
    if(( error_code = setjmp(entry_state) )){
         e_handle_error(error_code);
        return;
    }
    (*func)(argc, argv);
    return;
}


void e_error_nlf(e_error_t error_code){
    longjmp(entry_state, error_code);
}

void e_error_lf(e_error_t error_code, char * file, int line){
    e_file = file;
    e_line = line;
    e_error_nlf(error_code);
}
void e_error_lft(e_error_t error_code, char * file, int line, char* error_text){
    e_error_text_value = 1;
    e_error_text = error_text;
    e_error_lf(error_code, file, line);
}


static void e_handle_error(e_error_t error_code){
    IF_DEBUG fprintf(stderr, "ERROR:\n \tFILE: %s \tLINE: %d\nTRACE: \n",e_file, e_line);
    IF_DEBUG t_print_trace();

    switch(error_code){
        case ERROR:
            fprintf(stderr,"%d Error: Something happened\n", error_code);
            break;
        case ERROR_TEXT:
            fprintf(stderr, "Error: %s \n", e_error_text);
        case OPTIONS:
            fprintf(stderr, "Error: Invalid Options\n");
            break;
        case SYSTEM_CALL:
            e_handle_system_call();
            break;
        case MALLOC:
            fprintf(stderr, "Error: Malloc failed\n");
            break;
        case ARGUMENTS:
            fprintf(stderr, "Error: Invalid arguments\n");
            break;
        
        default:
            fprintf(stderr,"%d Error that is not definined occured, please update.\n", error_code);
    }
    return;
}

static void e_handle_system_call(){
    if(e_error_text_value){
        perror(e_error_text);
    } else {
        perror("Error:");
    }
    return;
}


