#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cmd_stor.h"
//#include "filemanip.h"
#include "net.h"
#include "path.h"
#include "response.h"
#include "session.h"


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


	//Determine if the pathname argument is correct and allowed.
	if ((pathCheck = check_futer_file(si->cwd, cmd)) == -1) {
	  send_mesg_450 (si->c_sfd);
	  return;
	} else if (pathCheck == -2) {
	  //improper return code, REDO
	  send_mesg_553 (si->c_sfd);
	  return;
	} else if (pathCheck == -3) {
	  send_mesg_553 (si->c_sfd);
	  return;
	}


	//if client is anonymous or they haven't logged in, they don't
	//have permission to run this command
	if (si->logged_in == false || strcmp(si->user,"anonymous") == 0) {
		char *permdeny = "550 Permission denied.\n";
		send_all(si->c_sfd,(uint8_t*)permdeny,strlen(permdeny));
		return;
	}


	/* Merge all pathname fragments to create a single pathname to use with
	 * fopen(). */
	char *fullPath;
	if ((fullPath = merge_paths(si->cwd, cmd, NULL)) == NULL) {
	  send_mesg_450 (si->c_sfd);
	  return;
	}
	if ((storfile = fopen(fullPath,purp)) == NULL) {
	  fprintf (stderr, "%s: fopen: %s\n", __FUNCTION__, strerror (errno));
	  free(fullPath);
	  send_mesg_450 (si->c_sfd);
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

	//data connection must already exist
	if (si->d_sfd == 0) {
		char *nodata = "425 Use PORT or PASV first.\n";
		send_all(si->c_sfd,(uint8_t*)nodata,strlen(nodata));
		return;
	}

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
