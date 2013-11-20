#ifndef __SESSION_H__
#define __SESSION_H__

#include <stdbool.h>


typedef struct {
	int c_sfd;				//control socket, rx from main
	int d_sfd;				//data socket, created from command thread
	char cwd[4096];			//current working directory
	char user[128];			//username
	bool logged_in;			//whether user is logged in
	bool cmd_complete;		//command thread is complete
	bool cmd_abort;			//command to abort
	bool cmd_quit;			//command to quit has been given
	char cmd_string[4096]	//command string for current command
}session_info_t;


#endif //__SESSION_H__
