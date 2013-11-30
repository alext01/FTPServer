/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: cmd_stor.h
 * Date: November 2013
 *
 * Description:
 *   FTP commands that deal with writing files to the file system:
 *   STOR, APPE, STOU
 *****************************************************************************/

#ifndef __CMD_STOR_H__
#define __CMD_STOR_H__

#include "session.h"


#define BUFFSIZE 1000


/******************************************************************************
 * Stores a file with a unquie filename.  Ignores parameter.
 *
 * Arguments:
 *   si - info for current session
 *   arg - parameter
 *
 *
 * Returns
 * 	void
 *
 * Original author: Justin Slind
 *****************************************************************************/
void cmd_stou (session_info_t *si, char *arg);

/******************************************************************************
 * Stores a file
 *
 * Arguments:
 *   si - info for current session
 *   cmd - current command with parameter
 *
 *
 * Returns
 * 	void
 *
 * Original author: Justin Slind
 *****************************************************************************/
void cmd_stor(session_info_t *si, char *cmd);

/******************************************************************************
 * Appends a file
 *
 * Arguments:
 *   si - info for current session
 *   cmd - current command with parameter
 *
 * Returns
 * 	void
 *
 * Original author: Justin Slind
 *****************************************************************************/
void cmd_appe(session_info_t *si, char *cmd);

/******************************************************************************
 * Stores or appends
 *
 * Arguments:
 *   cmd - current command with parameter
 *   si - info for current session
 *
 * Returns
 * 	head of the queue
 *
 * Original author: Justin Slind
 *****************************************************************************/
void store(session_info_t *si, char *cmd, char *purp);


#endif //__CMD_STOR_H__
