#include "files.h"
#include "utility.h"
#include "error.h"
#include "trace.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


void f_open_one(char * filename, unsigned char ** dest, size_t * size)
{
    IF_DEBUG t_push("f_open_one(%s, %p, %d)",filename,dest,size);

    int file;
    int in;
    struct stat file_stat;

    file = open(filename, O_RDONLY);
    if(file < 0) e_error(SYSTEM_CALL);


    if(fstat(file,&file_stat) < 0) e_error(SYSTEM_CALL);

    //initilize memory to store bytes
    *dest = malloc(sizeof(char) * file_stat.st_size);
    if(*dest < 0) e_error(MALLOC);

    *size = file_stat.st_size;

    in = read(file, (*dest) , file_stat.st_size);
    if(in == -1) e_error(SYSTEM_CALL);
    if(in != file_stat.st_size) e_error_t(ERROR_TEXT, "For some reason we didn't read in the whole file.");

    if(close(file) < 0) e_error(SYSTEM_CALL);

    IF_DEBUG t_pop();
}


void f_open_two(char * filename_1, unsigned char ** dest_1, size_t * size_1,
                char * filename_2, unsigned char ** dest_2, size_t * size_2)
{
    IF_DEBUG t_push("f_open_two(%s, %p, %d, %s, %p, %d)",filename_1,dest_1,size_1,filename_1,dest_1,size_1);

    int file_1, file_2;
    int in_1, in_2;
    struct stat file_stat_1;
    struct stat file_stat_2;

    file_1 = open(filename_1, O_RDONLY);
    file_2 = open(filename_2, O_RDONLY);
    if((file_1 < 0) || (file_2 < 0)) e_error(SYSTEM_CALL);

    if(fstat(file_1,&file_stat_1) < 0) e_error(SYSTEM_CALL);
    if(fstat(file_2,&file_stat_2) < 0) e_error(SYSTEM_CALL);

    //initilize memory to store bytes
    *dest_1 = malloc(sizeof(unsigned char) * file_stat_1.st_size);
    *dest_2 = malloc(sizeof(unsigned char) * file_stat_2.st_size);
    if((*dest_1 < 0) || (*dest_2 < 0)) e_error(MALLOC);

    *size_1 = file_stat_1.st_size;
    *size_2 = file_stat_2.st_size;

    in_1 = read(file_1,(*dest_1), file_stat_1.st_size);
    in_2 = read(file_2,(*dest_2), file_stat_2.st_size);

    if((in_1 == -1) || (in_2 == -1)) e_error(SYSTEM_CALL);

    if(in_1 != file_stat_1.st_size) e_error_t(ERROR_TEXT, "For some reason we didn't read in the whole file_1.");
    if(in_2 != file_stat_2.st_size) e_error_t(ERROR_TEXT, "For some reason we didn't read in the whole file_2.");

    if(close(file_1) < 0) e_error(SYSTEM_CALL);
    if(close(file_2) < 0) e_error(SYSTEM_CALL);

    IF_DEBUG t_pop();
}