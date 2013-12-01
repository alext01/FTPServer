/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: cmd_stor.c
 * Date: November 2013
 *
 * Description:
 *   FTP commands that deal with writing files to the file system:
 *   STOR, APPE, STOU
 *
 *   Code Citations:
 *   http://stackoverflow.com
 *****************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "cmd_stor.h"
#include "net.h"
#include "path.h"
#include "response.h"
#include "session.h"


//Local function prototypes.
static int perm_neg_check (session_info_t *si, char *arg);


void cmd_stou(session_info_t *si, char *arg) {
	//if client is anonymous or they haven't logged in, they don't
	//have permission to run this command
	if (si->logged_in == false || strcmp(si->user,"anonymous") == 0) {
		char *permdeny = "550 Permission denied.\n";
		send_all(si->c_sfd,(uint8_t*)permdeny,strlen(permdeny));
		close(si->d_sfd);
		si->d_sfd = 0;
		return;
	}


	//RFC 959 does not require or expect parameter for STOU cmd.
	//This implementation ignores parameter to allow
	//modern ftp clients to work with STOU.

		srand(time(NULL));
		char tempname[256];
		char *fullPath = NULL;
		int rt;
		do {
			if (fullPath)
				free(fullPath);
			sprintf(tempname,"%d",rand());
			printf("%s\n",tempname);
			fullPath = merge_paths(si->cwd,tempname,NULL);
			rt = access(fullPath, F_OK);
		} while ((rt != -1));// && ; //verify that random name doesn't exist
		free(fullPath);
		store(si,tempname,"w");


	return;
}

void cmd_stor(session_info_t *si, char *cmd) {
        if (perm_neg_check (si, cmd) == -1)
                return;

	store(si,cmd,"w");
	return;
}

void cmd_appe(session_info_t *si, char *cmd) {
       if (perm_neg_check (si, cmd) == -1)
                return;

	store(si,cmd,"a");
	return;
}

void store(session_info_t *si, char *cmd, char *purp) {


	struct timeval timeout;
	fd_set rfds;
	FILE *storfile;
	int rt = -1;
	char buffer[BUFFSIZE];


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


	/* Merge all pathname fragments to create a single pathname to use with
	 * fopen(). */
	char *fullPath;
	if ((fullPath = merge_paths(si->cwd, cmd, NULL)) == NULL) {
	  cleanup_stor_recv (si, NULL, 451);
	  return;
	}
	printf("fullPath: %s\n",fullPath);
	if ((storfile = fopen(fullPath,purp)) == NULL) {
	  fprintf (stderr, "%s: fopen: %s\n", __FUNCTION__, strerror (errno));
	  free (fullPath);
	  cleanup_stor_recv (si, NULL, 451);
	}
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
			cleanup_stor_recv (si, storfile, 451);
                        return;
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
	cleanup_stor_recv (si, storfile, 0);
	return;
}


/******************************************************************************
 * Determine if the STOR or APPE argument should be permanently rejected.
 *
 * Arguments:
 *     si -
 *   path - The pathname argument passed to the command function.
 *
 * Return values:
 *   0    Accept the filename argument.
 *  -1    Reject the filename argument.
 *****************************************************************************/
static int perm_neg_check (session_info_t *si, char *arg)
{
	  int pathCheck;
        //if client is anonymous or they haven't logged in, they don't
	//have permission to run this command
	if (si->logged_in == false || strcmp(si->user,"anonymous") == 0) {
		char *permdeny = "550 Permission denied.\n";
		send_all(si->c_sfd,(uint8_t*)permdeny,strlen(permdeny));
		close(si->d_sfd);
		si->d_sfd = 0;
		return -1;
	}

	//Determine if the pathname argument should be accepted.
	if ((pathCheck = check_futer_file(si->cwd, arg, false)) == -1) {
	  cleanup_stor_recv (si, NULL, 450);
	  return -1;
	} else if (pathCheck == -2) {
	  cleanup_stor_recv (si, NULL, 553);
	  return -1;
	} else if (pathCheck == -3) {
	  cleanup_stor_recv (si, NULL, 553);
	  return -1;
	}

	return 0;
}

/******************************************************************************
 * cleanup_stor_recv - see cmd_stor.h
 *****************************************************************************/
void cleanup_stor_recv (session_info_t *si, FILE *fp,  int errcode)
{
  if (errcode == 451) {
    send_mesg_451 (si->c_sfd);
  } else if (errcode == 553) {
    send_mesg_553 (si->c_sfd);
  } else if (errcode == 450) {
    send_mesg_450 (si->c_sfd);
  }

  if (fp != NULL)
    fclose (fp);
  close (si->d_sfd);
  si->d_sfd = 0;
}

