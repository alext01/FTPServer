#ifndef __CMD_STOR_H__
#define __CMD_STOR_H__

#include "session.h"


#define BUFFSIZE 1000

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
