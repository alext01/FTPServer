#include <stdio.h>
#include <string.h>
#include "cmd_stor.h"
//#include "filemanip.h"
#include "session.h"
#include "net.h"

void cmd_stor(session_info_t *si, char *cmd) {
	store(si,cmd,"w");
	return;
}

void cmd_appe(session_info_t *si, char *cmd) {
	store(si,cmd,"a");
	return;
}

void store(session_info_t *si, char *cmd, char *purp) {

	//if client is anonymous or they haven't logged in, they don't
	//have permission to run this command
	if (si->logged_in == false || strcmp(si->user,"anonymous") == 0) {
		char *permdeny = "550 Permission denied.\n";
		send_all(si->c_sfd,(uint8_t*)permdeny,strlen(permdeny));
		return;
	}
	if (si->d_sfd == 0) {
		char *nodata = "425 Use PORT or PASV first.\n";
		send_all(si->c_sfd,(uint8_t*)nodata,strlen(nodata));
		return;
	}
	FILE *storfile;
	storfile = fopen 	("testfile",purp);


	//do stuff

	fclose(storfile);
}
