#include "general.h"
#include "error.h"
#include "trace.h"

#include <unistd.h>

#include <stdio.h>

void g_ham_dist(const unsigned char * array_1, int size_1, const unsigned char * array_2, int size_2)
{
    IF_DEBUG t_push("g_ham_dist(%p, %d, %p , %d)", array_1, size_1, array_2, size_2);
    if(size_1 != size_2)
    {
        printf("ftor: -H: file sizes differ\n");
        return;
    }

    int dist = 0;
    unsigned char diff;

    for(int i = 0; i < size_1; i++)
    {
        diff = array_1[i]^array_2[i];
        while(diff)
        {
            diff &= diff -1;
            dist++;
        }
    }
    
    printf("ftor: -H: Hamming distance: %d\n", dist);
    IF_DEBUG t_pop();
}

void g_invert(const unsigned char * array, int size){
    IF_DEBUG t_push("g_invert(%p, %d)",array, size);

    for(int i = size-1; i >= 0; i--){
        if(write(1, &(array[i]), 1) < 0) e_error(SYSTEM_CALL);
    }

    IF_DEBUG t_pop();
}

void g_size(int size)
{
    IF_DEBUG t_push("g_size(%d)",size);

    printf("ftor: -s: %d bytes\n",size);

    IF_DEBUG t_pop();
}