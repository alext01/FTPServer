#ifndef __USERS_H__
#define __USERS_H__

#include "md5.h"

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


/******************************************************************************
 * finds an md5 based on the provided password and the username
 *
 * Arguments:
 *   user - username string
 *   password - password string
 *   md5checksum - string to hold checksum
 *
 * Returns
 * 	0
 *
 * Original author: Justin Slind
 *****************************************************************************/
int getMD5(char *user, char *password, char md5checksum[]);

#endif //__USERS_H__
