
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "cmd_misc.h"
#include "session.h"
#include "net.h"


void cmd_type(session_info_t *si, char *arg) {

	char *fail = "504 Command not implemented for that parameter.\n";
	char *ascii = "200 Switching to ASCII mode.\n";
	//must log in to change type
	if (!si->logged_in) {
		char *notloggedin = "530 Please login with USER and PASS.\n";
		send_all(si->c_sfd,(uint8_t*)notloggedin,strlen(notloggedin));
		return;
	}

	if (!arg) {
		char *syntaxerror = "501 Syntax error in arguments.\n";
		send_all(si->c_sfd, (uint8_t*)syntaxerror, strlen(syntaxerror));
		return;
	}

	// if arg is 1 char it must be an a or i, case insensitive
	if (strlen(arg) == 1) {
		arg[0] = tolower(arg[0]); //change arg to lowercase
		if (arg[0] == 'a') {

			send_all(si->c_sfd,(uint8_t*)ascii,strlen(ascii));
			si->type = 'a';
		} else if (arg[0] == 'i') {
			char *image = "200 Switching to Binary mode.\n";
			send_all(si->c_sfd,(uint8_t*)image,strlen(image));
			si->type = 'i';
		} else {
			send_all(si->c_sfd,(uint8_t*)fail,strlen(fail));
		}
		return;
	} if (strlen(arg) == 3) {
		//set interesting chars to lowercase
		if (strcasecmp(arg,"a n") == 0) {
			send_all(si->c_sfd,(uint8_t*)ascii,strlen(ascii));
			si->type ='a';
			return;
		}
	}
	send_all(si->c_sfd,(uint8_t*)fail,strlen(fail));
	return;
}
