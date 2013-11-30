#ifndef __CMD_STOR_H__
#define __CMD_STOR_H__

#include "session.h" //required for 'session_info_t' in function prototype.


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
 * modified by: Evan Myers
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
 * Modified by: Evan Myers
 *****************************************************************************/
void cmd_appe(session_info_t *si, char *cmd);


/******************************************************************************
 * Store a file with a unique name. If the filename argument received with the
 * client command is not unique, append a character to the filename and recheck.
 *
 * Arguments:
 *   si - info for current session
 *   cmd - current command with parameter
 *
 * Original author: Evan Myers
 *****************************************************************************/
void cmd_stou (session_info_t *si, char *arg);


#endif //__CMD_STOR_H__
