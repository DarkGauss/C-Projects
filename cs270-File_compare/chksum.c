#include "chksum.h"
#include "trace.h"
#include "error.h"

#include <stdio.h>


void c_parity_check(const unsigned char * array, int size)
{
    IF_DEBUG t_push("c_parity_check(%p, %d)", array, size);
    unsigned char parity = 0x00;
    for(int i = 0; i < size; i++)
    {
        parity ^= array[i];
    }
    printf("ftor: -x: %#04x\n",parity);
    IF_DEBUG t_pop();
}

void c_mod_sum(const unsigned char * array, int size)
{
    IF_DEBUG t_push("c_mod_sum(%p, %d)", array, size);
    unsigned char mod_sum = 0x00;
    //sum up every byte, overflowing at a 1 byte level
    for(int i = 0; i < size; i++)
    {
        mod_sum += array[i];
    }
    //calculate 2's complement
    mod_sum = ~mod_sum+1;
    printf("ftor: -X: %#04x\n",mod_sum);

    IF_DEBUG t_pop();
}