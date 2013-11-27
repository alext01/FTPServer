#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cmd_stor.h"
//#include "filemanip.h"
#include "session.h"
#include "net.h"
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

	storfile = fopen(cmd,purp);
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
	} else {
		char *success = "226 Closing data connection.\n";
		send_all(si->c_sfd,(uint8_t*)success,strlen(success));
	}

	//close file and data connection
	fclose(storfile);
	close(si->d_sfd);
	return;
}
