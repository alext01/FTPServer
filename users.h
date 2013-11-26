#ifndef __USERS_H__
#define __USERS_H__

/******************************************************************************
 * Check if username is valid
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
void cmd_user(session_info_t *si,char *cmd);


/******************************************************************************
 * checks if password matches for the previously given user
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
void cmd_pass(session_info_t *si,char *cmd);


#endif //__USERS_H__
