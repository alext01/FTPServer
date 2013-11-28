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


#define IS_CUR_NFILE 0   //This normal file should exist.
#define IS_CUR_DIR 1     //This directory should exist.
#define IS_FUTER_FILE 2  //All components of the prefix must be directories.


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
bool check_legal_path (char *argpath, int mode);


#endif //__PATH_H__
