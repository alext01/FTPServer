#include <string.h>
#include <strings.h>
#include "net.h"
#include "users.h"
#include "session.h"

void cmd_user(session_info_t *si,char *arg) {

	si->logged_in = false;
	if (strcasecmp(arg,"anonymous") == 0) {
		si->logged_in = true;

		char *loggedin = "230 Login successful.\n";
		send_all(si->c_sfd, (uint8_t*)loggedin, strlen(loggedin));
	} else if (arg != NULL) {
		char *needpass = "331 User name okay, need password.\n";
		send_all(si->c_sfd, (uint8_t*)needpass, strlen(needpass));
	}
	strcpy(si->user,arg);
	return;
}

void cmd_pass(session_info_t *si, char *cmd) {
	if (si->logged_in) {
		char *loggedin = "230 Already logged in.\n";
		send_all(si->c_sfd,(uint8_t*)loggedin, strlen(loggedin));
	}


	//check if username has been given

	//tokenize to get parameter

	//
	return;
}
