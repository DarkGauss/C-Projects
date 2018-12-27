/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//depth of call stack initialy
#define TRACEDEPTH_I 20

//length of text for each trace function
#define TRACETEXT_S 256


//see header for definitions
void t_setup_trace();
void t_push(const char *format, ...);
void t_pop();
void t_print_trace();

/**
 * t_expand_trace
 * --------------
 *  Expands the trace stack in incriments of 
 *  tracedepth and reallocates the memory
 * Return: none
 */
static void t_expand_trace();

//pointer to stack of traces
static char ** t_trace_stack;

//location of last trace added
static int t_last;

//stores size of tracestack
static int t_size;

void t_setup_trace(){
    //counter for loops
    int i;

    t_size = TRACEDEPTH_I;
    t_trace_stack = malloc(sizeof(char*) * TRACEDEPTH_I);
    if(t_trace_stack == 0) fprintf(stderr,"MALLOC FAILURE TRACE.C\n");
    t_last = -1;

    for(i = 0; i < TRACEDEPTH_I ; i++){
        t_trace_stack[i] = malloc(sizeof(char) * TRACETEXT_S);
        if(t_trace_stack[i] == 0) fprintf(stderr,"MALLOC FAILURE TRACE.C\n");
    }
    return;
}
void t_push(const char *format, ...){
    va_list args;
    t_last = t_last + 1;

    if(t_last >= t_size){
        t_expand_trace();
    }
    char * dest = t_trace_stack[t_last];
    //todo add code if we have overrun of trace stack

    va_start(args, format);
    vsnprintf(dest, TRACETEXT_S, format, args);
    va_end(args);
    return;
}

void t_pop(){
    t_last--;
    return;
}

void t_print_trace(){
    //counter for loops
    int i;

    for(i = 0; i <= t_last; i++){
        fprintf(stderr,"%d.\t%s\n", i, t_trace_stack[i]);
    }
    return;
}

static void t_expand_trace(){
    //counter var
    int i;
    int new_size = t_size + TRACEDEPTH_I;
    char ** new_stack = malloc(sizeof(char**) * new_size);
    if(new_stack == 0) fprintf(stderr,"MALLOC FAILURE TRACE.C\n");
    for(i = 0; i < new_size; i++){
        if(i < t_size){
            new_stack[i] = t_trace_stack[i];
        } else {
            new_stack[i] = malloc(sizeof(char) * TRACETEXT_S);
            if(new_stack[i] == 0) fprintf(stderr,"MALLOC FAILURE TRACE.C\n");
        }
    }
    t_size = new_size;
    free(t_trace_stack);
    t_trace_stack = new_stack;

}
