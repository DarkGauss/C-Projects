/**
 * Copywrite (2018) Robert Goes
 * Please messege me if you wish to use this code 
 */

#ifndef FILES_H
#define FILES_H

/**
 * f_rec_copy_src_dest
 * -------------------
 * Recursivly copies all files/ sub directories located
 * in src to dest. If directory's don't exist, an eror is raised.
 *  if Verbose option is set, detailed descriptions will be output
 *  if Backup option is set, backups will be created according to
 *      details in the specfic function
 *  if Confirm option is set, confirmation will be asked before copying
 *  if Force option is set, checking for diffrence between files will not
 *    be executed
 * 
 * Arguments: Source directory path, Destination Directory path
 * Returns: nothing
 */
void f_copy_src_dest(char*, char*);
#endif
