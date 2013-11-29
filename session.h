#ifndef __SESSION_H__
#define __SESSION_H__

#include <stdbool.h>
#include "queue.h"


#define CMD_STRLEN 4096
#define USER_STRLEN 64
#define ABORT_STRLEN 5
#define USEC_TIMEOUT 500



typedef struct {
	int c_sfd;				//control socket, rx from main
	int d_sfd;				//data socket, created from command thread
	char cwd[CMD_STRLEN];			//current working directory
	char user[USER_STRLEN];			//username
	bool logged_in;			//whether user is logged in
	bool cmd_complete;		//command thread is complete
	bool cmd_abort;			//command to abort
	bool cmd_quit;			//command to quit has been given
	char cmd_string[CMD_STRLEN];	//command string for current command
	char type;
}session_info_t;

/******************************************************************************
 * This functions handles all aspects of a client's session.  It recieves
 * commands via the control socket which is passed in and creates a command
 * thread to complete the command.
 *
 * Arguments:
 *   c_sfd - control socket file descriptor
 *   cmd_queu_ptr - queue of commands
 *
 * Returns
 * 	returns 0 when function is complete
 *
 * Original author: Justin Slind
 * Checked by: Evan Myers, Nov.26
 *****************************************************************************/

int session(int c_sfd);

//Checked by: Evan Myers, Nov.26
int readCmd(char *str,int sock, session_info_t *si);

#endif //__SESSION_H__
