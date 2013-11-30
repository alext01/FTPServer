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


//Local function prototype.
static void store(session_info_t *si, char *cmd, char *purp);


/******************************************************************************
 * cmd_stor - see cmd_stor.h
 *****************************************************************************/
void cmd_stor(session_info_t *si, char *cmd) {
        int pathCheck;

	//Determine if the pathname argument is correct and allowed.
        if ((pathCheck = check_futer_file (si->cwd, cmd, false)) != 0) {
	  send_mesg_450 (si->c_sfd);
	  return;
	} else if (pathCheck == -2) {
	  send_mesg_553 (si->c_sfd);
	  return;
	} else if (pathCheck == -3) {
	  send_mesg_553 (si->c_sfd);
	  return;
	}

	store(si,cmd,"w");
	return;
}


/******************************************************************************
 * cmd_appe - see cmd_stor.h
 *****************************************************************************/
void cmd_appe(session_info_t *si, char *cmd) {
        int pathCheck;

	//Determine if the pathname argument is correct and allowed.
        if ((pathCheck = check_futer_file (si->cwd, cmd, false)) != 0) {
	  send_mesg_450 (si->c_sfd);
	  return;
	} else if (pathCheck == -2) {
	  send_mesg_553 (si->c_sfd);
	  return;
	} else if (pathCheck == -3) {
	  send_mesg_553 (si->c_sfd);
	  return;
	}

	//Store the file.
	store(si,cmd,"a");
	return;
}


/******************************************************************************
 * cmd_stou - see cmd_stor.h
 *****************************************************************************/
void cmd_stou (session_info_t *si, char *arg) 
{
  int pathCheck;
  int arg_strlen;

  /* Determine if the argument would create a unique filename. If the path to
   * the file is accepted, but the filename is not unique, append a character
   * in an attempt to find a unique filename. */
  while ((pathCheck = check_futer_file (si->cwd, arg, true)) != 0) {
    //Error while processing.
    if (pathCheck == -1) {
      send_mesg_450 (si->c_sfd);
      return;

      //Illegal pathname.
    } else if (pathCheck == -2) {
      send_mesg_553 (si->c_sfd);
      return;

      //The name is not unique, attempt to make it unique by appending a char.
    } else if (pathCheck == -3) {
      if ((arg_strlen = strlen (arg) + 1) > CMD_STRLEN)
	return;
      arg[arg_strlen -1] = 'a'; //'a' chosen arbitrarily.
      continue;
    }
  }

  //The filename is unique, store the file.
  store (si, arg, "w");
  return;
}


/******************************************************************************
 * Stores or appends
 *
 * Arguments:
 *   cmd - current command with parameter
 *   si - info for current session
 *
 * Returns
 * 	head of the queue
 *
 * Original author: Justin Slind
 *****************************************************************************/
static void store(session_info_t *si, char *cmd, char *purp) {
	struct timeval timeout;
	fd_set rfds;
	FILE *storfile;
	int rt = -1;
	char buffer[BUFFSIZE];
	char *fullPath;


	//if client is anonymous or they haven't logged in, they don't
	//have permission to run this command
	if (si->logged_in == false || strcmp(si->user,"anonymous") == 0) {
		char *permdeny = "550 Permission denied.\n";
		send_all(si->c_sfd,(uint8_t*)permdeny,strlen(permdeny));
		return;
	}


	/* Merge all pathname fragments to create a single pathname to use with
	 * fopen(). */
	if ((fullPath = merge_paths(si->cwd, cmd, NULL)) == NULL) {
	  send_mesg_450 (si->c_sfd);
	  return;
	}
	if ((storfile = fopen (fullPath, purp)) == NULL) {
	  fprintf (stderr, "%s: fopen: %s\n", __FUNCTION__, strerror (errno));
	  free (fullPath);
	  send_mesg_450 (si->c_sfd);
	  return;
	}
	free (fullPath);

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
	si->d_sfd = 0;
	return;
}
