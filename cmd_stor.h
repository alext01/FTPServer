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
 * Note: This function will store the unique name to the current working
 *       directory and not any other path argument. This is intended, we were
 *       following "rfc 959".
 *
 * Original author: Justin Slind
 * Checked by: Evan Myers
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

/******************************************************************************
 * Close all sockets, reset stored socket file descriptor in the
 * session_info_t structure, and close the file pointer when appropriate.
 *
 * This function was created to help defend against programmer error. These
 * closing statements appear in many places in the stor command.
 *
 * Arguments:
 *   si - info for current session
 *   fp - the open filestream, set this to NULL if no filestream has been
 *        opened.
 *   errcode - The type of error. This should be set to zero if no error
 *             occured.
 *
 * Original author: Evan Myers
 *****************************************************************************/
void cleanup_stor_recv (session_info_t *si, FILE *fp, int errcode);

#endif //__CMD_STOR_H__
