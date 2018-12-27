#include "runs.h"
#include "trace.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>

//stores details about runs
struct run_entry{
    int start;
    unsigned char byte;
    int length;
};

/**
 * r_make_run_table
 * ----------------
 * Make table of all runs occouring in an array, ordered by their occurence
 * Args:
 *      char array, size
 */
static struct run_entry * r_make_run_table(const unsigned char * array, int size, int * number_runs);

/**
 * compare_run
 * -----------
 * comparision function for sorting runs, decending
 */
static int compare_run(const void * a, const void * b);

void r_run_longest(const unsigned char * array, int size)
{
    IF_DEBUG t_push("r_run_longest(%p, %d", array, size);
    //handle the case when the file is empty. I chose to structure the output the same way
    //in case someone was using the output of this program into some sort of parser
    if(size == 0)
    {
        printf("ftor: -r: 0 0x00 0\n");
        return;
    }
    int number_runs;    //stores the number of runs detected

    struct run_entry * table = r_make_run_table(array, size, &number_runs);
    qsort(table, number_runs, sizeof(struct run_entry), compare_run);
    
    printf("ftor: -r: %d %#04x %d\n", table[0].start, (unsigned int)table[0].byte, table[0].length);

    free(table);
    IF_DEBUG t_pop();
}

void r_run_all(const unsigned char * array, int size)
{
    IF_DEBUG t_push("r_run_all(%p, %d", array, size);
    if(size == 0)
    {
        printf("ftor: -r: 0 0x00 0\n");
        return;
    } 
    int number_runs;    //stores the number of runs detected
    
    struct run_entry * table = r_make_run_table(array, size, &number_runs);
    qsort(table, number_runs, sizeof(struct run_entry), compare_run);

    printf("ftor: -r:\n");
    for(int i = 0; i < number_runs; i++)
    {
        printf("\t%d %#04x %d\n", table[i].start, (unsigned int)table[i].byte, table[i].length);
    }    
    free(table);

    IF_DEBUG t_pop();
}

static struct run_entry * r_make_run_table(const unsigned char * array, int size, int * number_runs)
{
    IF_DEBUG t_push("fr_make_run_table(%p, %d)", array, size);
    
    int run_current = 0; //current postition in table
    
    //initialy assumme there is no runs > 1
    struct run_entry * output = malloc(sizeof(struct run_entry)*size);
    if(output < 0) e_error(MALLOC);

    //setup first run before loop, cleaner this way
    output[0].start = 0;
    output[0].byte = array[0];
    output[0].length = 1;
    
    
    for(int i = 1; i < size; i++)
    {
        if( array[i] == array[i-1])
        {
            ++(output[run_current].length);
        }
        else
        {
            output[++run_current].start = i;
            output[run_current].byte = array[i];
            output[run_current].length = 1;
        }
    }

    //shrink the size down to match the actual runs. 
    output = realloc(output, sizeof(struct run_entry)*(++run_current));
    if(output < 0) e_error(MALLOC);

    *number_runs = run_current;
    IF_DEBUG t_pop();
    return output;
}

static int compare_run(const void * a, const void * b)
{
    return ((struct run_entry *)b)->length - ((struct run_entry *)a)->length;
}