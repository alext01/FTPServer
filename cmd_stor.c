#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cmd_stor.h"
//#include "filemanip.h"
#include "session.h"
#include "net.h"
#include "path.h"
#include <unistd.h>

void cmd_stor(session_info_t *si, char *cmd) {
	store(si,cmd,"w");
	return;
}

void cmd_appe(session_info_t *si, char *cmd) {
	store(si,cmd,"a");
	return;
}

void store(session_info_t *si, char *cmd, char *purp) {

  int pathCheck;
	struct timeval timeout;
	fd_set rfds;
	FILE *storfile;
	int rt = -1;
	char buffer[BUFFSIZE];
	//if client is anonymous or they haven't logged in, they don't
	//have permission to run this command
	if (si->logged_in == false || strcmp(si->user,"anonymous") == 0) {
		char *permdeny = "550 Permission denied.\n";
		send_all(si->c_sfd,(uint8_t*)permdeny,strlen(permdeny));
		return;
	}
	//data connection must already exist
	if (si->d_sfd == 0) {
		char *nodata = "425 Use PORT or PASV first.\n";
		send_all(si->c_sfd,(uint8_t*)nodata,strlen(nodata));
		return;
	}

	//send positive prelimitary reply
	char *transferstart = "150 Opening ";
	char *middle = " mode data connection for ";
	char *type;
	send_all(si->c_sfd,(uint8_t*)transferstart,strlen(transferstart));
	if (si->type == 'a')
		type = "ASCII";
	else
		type = "BINARY";
	send_all(si->c_sfd,(uint8_t*)type,strlen(type));
	send_all(si->c_sfd,(uint8_t*)middle,strlen(middle));
	send_all(si->c_sfd,(uint8_t*)cmd,strlen(cmd));
	char *endln = ".\n";
	send_all(si->c_sfd,(uint8_t*)endln,strlen(endln));

	if ((pathCheck = check_futer_file(si->cwd, cmd)) == -1) {
	  send_mesg_451();
	} else if (pathCheck == -2) {
	  //improper return code, REDO
	  send_mesg_553();
	} else if (pathCheck == -3) {
	  send_mesg_553();
	}

	char *fullPath;
	if ((fullPath = merg_paths(si->cwd, cmd, NULL)) == NULL) {
	  return;
	}
	storfile = fopen(fullPath,purp);
	free(fullPath);
	while(si->cmd_abort == false && rt != 0) {
		FD_ZERO(&rfds);
		FD_SET(si->d_sfd,&rfds);
		timeout.tv_sec = 0;
		timeout.tv_usec = USEC_TIMEOUT;
		int sr = 0;
		if ((sr = select(si->d_sfd+1,&rfds,NULL,NULL,&timeout)) == -1) {
			if (errno == EINTR)
				continue;
			fprintf (stderr, "%s: select: %s\n", __FUNCTION__, strerror (errno));
		}
		//check for timeout.
		if (sr == 0)
			continue;

		//check if data port has rxed data
		if (FD_ISSET(si->d_sfd, &rfds)) {
		       if ((rt = recv(si->d_sfd,buffer,BUFFSIZE,0)) > 0) {
		    	   fwrite(buffer,sizeof(char),rt,storfile);
		       }


		}
	}

	if (si->cmd_abort) {
		char *aborted = "426 Connection closed; transfer aborted.\n";
		send_all(si->c_sfd,(uint8_t*)aborted,strlen(aborted));
		si->cmd_abort = false;
	} else {
		char *success = "226 Transfer Complete.\n";
		send_all(si->c_sfd,(uint8_t*)success,strlen(success));
	}

	//close file and data connection
	fclose(storfile);
	close(si->d_sfd);
	return;
}
