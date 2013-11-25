#include "users.h"
#include "session.h"

void cmd_user(char *cmd, session_info_t *si) {

	char username[USER_STRLEN];
	//tokenize to get parameter
	char *token = strtok(cmd, " ");
	token = strtok(NULL," ");
	strcpy(username,token);
	//check if user exisits in file

	//set session info user to username
	return;
}

void cmd_pass(char *cmd, session_info_t *si) {
	//check if username has been given

	//tokenize to get parameter

	//
	return;
}
