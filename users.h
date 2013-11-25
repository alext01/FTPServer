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
void cmd_user(char *cmd, session_info_t *si);


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
void cmd_pass(char *cmd, session_info_t *si);


#endif //__USERS_H__
