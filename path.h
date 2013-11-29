/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: path.h
 * Date: November 2013
 *
 * Description:
 *   The functions on this determine if a pathname argument to a client command
 *   is acceptable for the given function.
 *****************************************************************************/
#ifndef __PATH_H__
#define __PATH_H__


#include <stdbool.h> //Required for boolean return type in function prototype.


/******************************************************************************
 * check_file_exist
 *
 * -check if the path is within the scope of the root directory, and that a
 *  file given in the argument argpath exists.
 *
 * Return values:
 *   true - It is safe to RETR this file.
 *   false - It is not safe to RETR this file.
 *****************************************************************************/
bool check_file_exist (const char *cwd, const char *argpath);


/******************************************************************************
 * check_dir_exist
 *****************************************************************************/
bool check_dir_exist (const char *cwd, const char *argpath);


/******************************************************************************
 * check_futer_file
 *****************************************************************************/
int check_futer_file (const char *cwd, char *argpath);


/******************************************************************************
 * merge_paths: updating comments later. 
 *
 * ATTENTION: You must free the returned string!
 *
 * Arguments:
 *  cwd - current working directory string.
 *  argpath - The pathname argument received from the client.
 *  reserve - Will update this comment. If you have not previously called
 *            trim_arg_path() on the second argument to this function, reserve
 *            should be passed NULL.
 *
 * Return value:
 *   NULL - error
 *   string - The full pathname (rootdir -> cwd -> argument).
 *****************************************************************************/
char *merge_paths (const char *cwd, const char *argpath, const int *reserve);


#endif //__PATH_H__
