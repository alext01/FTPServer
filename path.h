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
#include "session.h" //Required for 'session_info_t' in function prototype.

#define CUR_FILE 0    //Determine if this file exists.
#define CUR_DIR 1     //Determine if this directory exists.
#define FUTER_FILE 2  //All components of the prefix must be directories.
/* All components of the prefix must be directories, and the file must not
 * already exist. */
#define FUTER_UNIQ 3 


/******************************************************************************
 * check_file_exist
 *****************************************************************************/
bool check_file_exist (const char *cwd, const char *argpath);


/******************************************************************************
 * check_dir_exist
 *****************************************************************************/
bool check_dir_exist (const char *cwd, const char *argpath);


/******************************************************************************
 * check_futer_file
 *****************************************************************************/
int check_futer_file (const char *cwd, const char *argpath);


/******************************************************************************
 * To be completed later. Quite a lot to be covered.
 *
 * For now this is all thats important to use this function:
 *      -argpath -> put your pathname argument here.
 *
 *      -mode    -> If you want to know if u can make a file or directory with
 *                  this pathname, use IS_FUTER_FILE. eg. STOR or STRU
 *
 *                  If you want to know if the path is a valid directory,
 *                  use IS_CUR_DIR. eg. CWD, NLST, LIST
 *
 *                  If you want to know if this is a file that you can send,
 *                  use IS_CUR_NFILE. eg. RETR
 *
 *
 * This function is not thoroughly tested. The method did return correct
 * results in a slightly shortened version, but I will thoroughly test this
 * tommorow.
 *
 * If this function returns true, it is safe to use the path for the requested
 * action. If it is false, you should abort the current action. This function
 * sends the appropriate code to the control socket.
 *****************************************************************************/
bool accept_path (session_info_t session, char *argpath, int mode);


#endif //__PATH_H__
