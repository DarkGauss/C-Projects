/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */

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

//maxiumum path length
#define PATH_LENGTH 1024
//maximum bufferlenth
#define BUFSIZE 512

//see header for definitions
void f_copy_src_dest(char * source, char* dest);

/**
 * f_copy_dir
 * ------------
 *  Transverses source directory provided. 
 *  If the entry is a directory, and it doesnt exist in destination
 *  it is created. 
 *  If the entry is a file, and it doesnt exist in the destination,
 *     it is copied
 *  if the entry is a file, and it does exist in the destination
 *      it is copied with f_copy_file_e which does required checks
 * 
 */
static void f_copy_dir(char * source_path, DIR * source, char * dest_path, DIR * dest);



/**
 * f_copy_file_e
 * -----------
 *  Copies file from specfied path to dest path if it already exists
 * if:
 *      sizes differ
 *          file is copied 
 *      sizes same
 *          if no -f, contents are scanned
 *          and only copied if differ
 *      -c option
 *          confirmation is asked
 *      -B option
 *          backup is created
 */         
static void f_copy_file_e(char * source_path, char * source_name, char * dest_path);


/**
 * f_copy_file
 * -----------
 *  Copies file from specfied path to dest path and overwrites it if it allready exists
 */  
static void f_copy_file(char * source_path, char * source_name, char * dest_path, char * dest_name);


/**
 * f_make_backup
 * -------------
 * Makes a backup copy of file 
 */
static void f_make_backup(char* source_path, char* source_name);

/**
 * f_check_diffrent
 * ----------------
 *  Compares the contents of two files, 
 *  if they are diffrent, reuturns 1, else 0
 */
static int f_check_diffrent(char * src_full, char* dst_full);

/**
 * f_confirm_copy
 * --------------
 *  Asks user if they want to perform copy, if y or Y
 *  a 1 is returned, else 0
 */
static int f_confirm_copy(char* src_full, char* dst_full);



void f_copy_src_dest(char * source, char* dest){
    IF_DEBUG t_push("f_rec_copy_src_dest(%s, %s)", source, dest);

    DIR *source_dir, *dest_dir;

    source_dir = opendir(source);
    if(source_dir == NULL) e_error_t(SYSTEM_CALL, "Source");

    dest_dir = opendir(dest);
    if(dest_dir == NULL) e_error_t(SYSTEM_CALL, "Dest");

    f_copy_dir(source, source_dir, dest, dest_dir);

    //f_copy_file("./test_src","test_file_1","test_dest");

    IF_DEBUG t_pop();
    return;
}

static void f_copy_dir(char * source_path, DIR * source, char * dest_path, DIR * dest){
    IF_DEBUG t_push("f_copy_dir(%s, %p, %s, %p)", source_path, source, dest_path, dest);

    struct dirent *source_entry, *dest_entry;
    
    //stores wheater file wa found in dest
    int in_dest = 0;
    /*
     * for every item in source directory
     * compare it to the every file in the dest
     * and if the names match, begin copy proccess
     * 
     * warning, gets screwy if you name dirs same as file
     */
    while((source_entry = readdir(source)) != NULL){
        //handle the case when it is a file
        if(source_entry -> d_type == DT_REG){
            in_dest = 0;
            while((dest_entry = readdir(dest)) != NULL){
                if(strcmp(source_entry -> d_name, dest_entry -> d_name) == 0){
                    f_copy_file_e(source_path, source_entry->d_name, dest_path);
                    in_dest = 1;
                    break;
                }
            }
            rewinddir(dest);
            if(!in_dest){
                if(u_chk_mode(Verbose)){
                    fprintf(stderr,"%s doesn't exist in %s\n",source_entry->d_name, dest_path);
                }
                f_copy_file(source_path, source_entry->d_name, dest_path, source_entry->d_name);
            }
        //handle the case when it's a dir
        } else if(source_entry -> d_type == DT_DIR      &&
            !(strcmp(source_entry -> d_name, ".") == 0) &&
            !(strcmp(source_entry -> d_name, "..") == 0) ){

            char * src_full = calloc(PATH_LENGTH,sizeof(char));
            char * dst_full = calloc(PATH_LENGTH,sizeof(char));
            if( (src_full == 0) || (dst_full == 0)) e_error(MALLOC);
        
            //store full paths for source dir and dest dir
            sprintf(src_full,"%s/%s",source_path, source_entry->d_name);
            sprintf(dst_full,"%s/%s",dest_path, source_entry-> d_name);

            in_dest = 0;
            while((dest_entry = readdir(dest)) != NULL){
                if(strcmp(source_entry -> d_name, dest_entry -> d_name) == 0){
                    if(dest_entry-> d_type != DT_DIR){
                        e_error_t(ERROR_TEXT,"A file in the destination was the same name as a src directory.");
                    }
                    in_dest = 1;
                    f_copy_src_dest(src_full, dst_full);
                    break;
                }
            }
            rewinddir(dest);

            //we need to create it there then
            if(!in_dest){
                if(u_chk_mode(Verbose)){
                    fprintf(stderr,"%s doesn't exist in %s\n",source_entry->d_name, dest_path);
                }
                if(mkdir(dst_full, 0777) == -1) e_error(SYSTEM_CALL);
                f_copy_src_dest(src_full, dst_full);
            }
            free(src_full);
            free(dst_full);
        }
    }
    rewinddir(source);
    IF_DEBUG t_pop();
    return;
}


static void f_copy_file_e(char * source_path, char * source_name, char * dest_path){
    IF_DEBUG t_push("f_copy_file_e(%s, %s, %s)",source_path, source_name, dest_path);


    struct stat src_stat, dst_stat;

    char * src_full = calloc(PATH_LENGTH,sizeof(char));
    char * dst_full = calloc(PATH_LENGTH,sizeof(char));
    if( (src_full == 0) || (dst_full == 0)) e_error(MALLOC);

    //store full paths for source file and dest file
    sprintf(src_full,"%s/%s",source_path, source_name);
    sprintf(dst_full,"%s/%s",dest_path, source_name);

    if(stat(src_full, &src_stat) == -1) e_error(SYSTEM_CALL);
    if(stat(dst_full, &dst_stat) == -1) e_error(SYSTEM_CALL);
    

    if((src_stat.st_size == dst_stat.st_size) && !u_chk_mode(Force)){
        if(u_chk_mode(Verbose)){
            fprintf(stderr,"\"%s\" is the same size as \"%s\" \n",src_full, dst_full);
        }
        if(f_check_diffrent(src_full, dst_full)){
            if(u_chk_mode(Confirm)){
                if(f_confirm_copy(src_full, dst_full)){
                    if(u_chk_mode(Backup)){
                        fprintf(stderr, "making backup\n");
                        f_make_backup(dest_path, source_name);
                    }
                    f_copy_file(source_path, source_name, dest_path, source_name);
                }
            } else {
                if(u_chk_mode(Backup)){
                    fprintf(stderr, "making backup\n");
                    f_make_backup(dest_path, source_name);
                }
                f_copy_file(source_path, source_name, dest_path, source_name);
            }
        }
    }else {

        if(u_chk_mode(Confirm)){
            if(f_confirm_copy(src_full, dst_full)){
                if(u_chk_mode(Backup)){
                    fprintf(stderr, "making backup\n");
                    f_make_backup(dest_path, source_name);
                }
                f_copy_file(source_path, source_name, dest_path, source_name);
            }
        } else {
            if(u_chk_mode(Backup)){
                fprintf(stderr, "making backup\n");
                f_make_backup(dest_path, source_name);
            }
            f_copy_file(source_path, source_name, dest_path, source_name);
        }
    }

    free(src_full);
    free(dst_full);
    IF_DEBUG t_pop();
    return;
}


static void f_copy_file(char * source_path, char * source_name, char * dest_path, char * dest_name){
    IF_DEBUG t_push("f_copy_file(%s, %s, %s)",source_path, source_name, dest_path);

    //file descriptors for source and dest
    int src, dst;

    //store counts of files in and out
    int in, out;

    char * src_full = calloc(PATH_LENGTH,sizeof(char));
    char * dst_full = calloc(PATH_LENGTH,sizeof(char));
    if( (src_full == 0) || (dst_full == 0)) e_error(MALLOC);

    //store full paths for source file and dest file
    sprintf(src_full,"%s/%s",source_path, source_name);
    sprintf(dst_full,"%s/%s",dest_path, dest_name);

    if(u_chk_mode(Verbose)){
        fprintf(stderr,"Attempting to copy \"%s\" to \"%s\" \n",src_full, dst_full);
    }
    src = open(src_full, O_RDONLY);
    dst = open(dst_full, O_WRONLY | O_CREAT |O_TRUNC);
    if( (src < 0) || (dst < 0)) e_error(SYSTEM_CALL);

    char buffer[BUFSIZE];

    while(1){
        in = read(src, buffer, BUFSIZE);
        if( in == 0) break;
        if(in == -1) e_error(SYSTEM_CALL);

        out = write(dst, buffer, in);
        if( out == 0) break; 
        if( out == -1) e_error(SYSTEM_CALL);
    }
    
    if(close(src) < 0) e_error(SYSTEM_CALL);
    if(close(dst) < 0) e_error(SYSTEM_CALL);

    if(u_chk_mode(Verbose)){
        fprintf(stderr,"Succesfully copied \"%s\" to \"%s\" \n",src_full, dst_full);
    }
    
    free(src_full);
    free(dst_full);
    IF_DEBUG t_pop();
    return;
}


static void f_make_backup(char* source_path, char* source_name){
    IF_DEBUG t_push("f_make_backup(%s, %s)",source_path, source_name);

    time_t raw_time;
    struct tm *info;
    char * new_name = calloc(PATH_LENGTH, sizeof(char));
    if(new_name == NULL) e_error(MALLOC);

    raw_time = time(NULL);
    info = localtime(&raw_time);

    //appends the following to the name: .ddup-HH_MM_SS-YYYY.MM.DD
    //corrects month so 1 = jan, etc
    sprintf(new_name,"%s.ddup-%d_%d_%d-%d.%d.%d", source_name, info->tm_hour, info->tm_min, info->tm_sec
                                                , (1900+ info ->tm_year), (info->tm_mon + 1), (info->tm_mday)  );
    if(u_chk_mode(Verbose)){
            fprintf(stderr,"Creating backup \"%s\"\n",new_name);
        }
    f_copy_file(source_path, source_name, source_path, new_name);
    free(new_name);

    IF_DEBUG t_pop();
    return;

}

static int f_check_diffrent(char * src_full, char* dst_full){
    IF_DEBUG t_push("f_check_diffrent(%s, %s)", src_full, dst_full);

    if(u_chk_mode(Verbose)){
        fprintf(stderr,"Checking if \"%s\" is diffrent than \"%s\" \n",src_full, dst_full);
    }

    //file descriptors for source and dest
    int src, dst;
    int src_in, dst_in;
    int is_diff = 0;
    int i = 0;
    char src_buffer[BUFSIZE];
    char dst_buffer[BUFSIZE];

    src = open(src_full, O_RDONLY);
    dst = open(dst_full, O_RDONLY);
    if( (src < 0) || (dst < 0)) e_error(SYSTEM_CALL);

    while(1){
        src_in = read(src, src_buffer, BUFSIZE);
        dst_in = read(dst, dst_buffer, BUFSIZE);
        if(src_in == -1 || dst_in == -1) e_error(SYSTEM_CALL);

        if(src_in != dst_in){
            is_diff = 1;
            break;
        }
        //doesnt matter which we check since they are the same
        if( src_in == 0) break;
        for(i = 0; i < src_in; i++){
            if(src_buffer[i] != dst_buffer[i]){
                is_diff = 1;
                break;
            }
        }
        if(is_diff == 1) break; 
    }

    if(u_chk_mode(Verbose)){
        if(is_diff) fprintf(stderr,"They are diffrent!\n");
        if(!is_diff) fprintf(stderr,"They are the same!\n"); 
    }

    if(close(src) < 0) e_error(SYSTEM_CALL);
    if(close(dst) < 0) e_error(SYSTEM_CALL);

    IF_DEBUG t_pop();
    return is_diff;
}

static int f_confirm_copy(char* src_full, char* dst_full){
    IF_DEBUG t_push("f_confirm_copy(%s, %s)", src_full, dst_full);

    char input_buf[100];
    fprintf(stderr, "Copy %s to %s ? [yn]:", src_full, dst_full);
    //handle the case the user doesn't input anything
    if(fgets(input_buf, 100, stdin) == 0) input_buf[0] = 'n';
    if(!(input_buf[0] == 'y' || input_buf[0] == 'Y')){
        fprintf(stderr, "skipped copy\n");
        IF_DEBUG t_pop();
        return 0;
    }
    IF_DEBUG t_pop();
    return 1;
}
