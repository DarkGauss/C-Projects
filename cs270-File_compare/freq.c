#include "freq.h"
#include "error.h"
#include "trace.h"

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

//stores a byte and it's frequency
struct freq_entry{
    unsigned char byte;
    int freq;
};

/**
 * fr_make_freq_table
 * ------------------
 * Makes a table for every possible byte with an entry with it's frequency and the byte
 * Args:
 *      array of bytes, size of the array
 */
static struct freq_entry * fr_make_freq_table(const unsigned char * array, int size);

/**
 * compare_freq
 * ------------
 * used as comparision function for qsort
 */
static int compare_freq(const void * a, const void * b);

void fr_freq_unsorted(const unsigned char * array, int size)
{
    IF_DEBUG t_push("fr_freq_unsorted(%p, %d", array, size);

    int possible_bytes = 1<<CHAR_BIT;

    printf("byte freq\n==== ====\n");

    struct freq_entry * table = fr_make_freq_table(array, size);

    for(int i = 0; i < possible_bytes; i++)
    {
        if(table[i].freq > 0) printf("%#04x %d\n",i, table[i].freq);
    }
    free(table);
    IF_DEBUG t_pop();
}

void fr_freq_sorted(const unsigned char * array, int size)
{
    IF_DEBUG t_push("fr_freq_sorted(%p, %d", array, size);

    int possible_bytes = 1<<CHAR_BIT;
    
    printf("byte freq\n==== ====\n");
    
    struct freq_entry * table = fr_make_freq_table(array, size);
    qsort(table, possible_bytes, sizeof(struct freq_entry), compare_freq);

    for(int i = 0; i < possible_bytes; i++)
    {
        if(table[i].freq > 0) printf("%#04x %d\n",(unsigned int)table[i].byte, table[i].freq);
    }
    free(table);
    IF_DEBUG t_pop();
}

struct freq_entry * fr_make_freq_table(const unsigned char * array, int size)
{
    IF_DEBUG t_push("fr_make_freq_table(%p, %d)", array, size);

    int possible_bytes = 1<<CHAR_BIT;
    struct freq_entry * output = malloc(sizeof(struct freq_entry)*possible_bytes);
    if(output < 0) e_error(MALLOC);

    //initialize the array to zero
    for(int i = 0; i < possible_bytes; i++)
    {
        output[i].byte = (unsigned char)i;
        output[i].freq = 0;
    }
    for(int i = 0; i < size; i++)
    {
        output[(int)array[i]].freq++;
    }
    IF_DEBUG t_pop();
    return output;
}

static int compare_freq(const void * a, const void * b)
{
    return ((struct freq_entry *)b)->freq - ((struct freq_entry *)a)->freq;
}