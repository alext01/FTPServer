#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include "session.h"
#include "queue.h"
#include "cmd_switch.h"

#include <stdio.h>



extern int shutdown_server;

int session(int c_sfd) {

	queue *cmd_queue_ptr = NULL;
	pthread_t command_thread = 0;
	session_info_t sessioninfo;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	char commandstr[CMD_STRLEN];
	struct timeval timeout;
	fd_set rfds;

	//init sessioninfo
	sessioninfo.c_sfd = c_sfd;
	sessioninfo.d_sfd = 0;
	sessioninfo.cmd_abort = false;
	sessioninfo.cmd_quit = false;
	sessioninfo.logged_in = false;
	sessioninfo.cmd_complete = false;
	sessioninfo.user[0] = '\0';
	sessioninfo.cmd_string[0] = '\0';
	sessioninfo.type = 'a';
	strcpy(sessioninfo.cwd,"/");

	commandstr[0] = '\0';



	//check if the server is shutting down or if the quit cmd was received from the client
	while (!shutdown_server && !sessioninfo.cmd_quit) {

		FD_ZERO(&rfds);
		FD_SET(c_sfd,&rfds);
		timeout.tv_sec = 0;
		timeout.tv_usec = USEC_TIMEOUT;

		//read from socket with timeout
		if (select(c_sfd+1,&rfds,NULL,NULL,&timeout) == -1) {
		        if (errno == EINTR)
		                continue;
		        fprintf (stderr, "%s: select: %s\n", __FUNCTION__, strerror (errno));
			freeQueue(cmd_queue_ptr);
		        return -1;
		}


		//if there's anything to read on the control socket, do so.
		if (FD_ISSET(c_sfd, &rfds)) {
		        if (readCmd(commandstr, c_sfd, &sessioninfo) == -1) {
		               freeQueue(cmd_queue_ptr);
			       return -1;
		        }

			printf("rxed: %s\n",commandstr);
			cmd_queue_ptr = addToQueue(commandstr, cmd_queue_ptr);
		}

		//if command is abort (ABOR) let the current thread know
		if (strncasecmp(commandstr,"ABOR",4) == 0) {
			printf("Abort was set\n");
			sessioninfo.cmd_abort = true;
			commandstr[0] = '\0';
		}


		//if there isn't a command_thread already, create one to either handle a command
		//on the command_queue or the current command
		else if (command_thread == 0 && cmd_queue_ptr) {
			if (cmd_queue_ptr) {

				cmd_queue_ptr = pullFromQueue(commandstr, cmd_queue_ptr);
			}

			strcpy(sessioninfo.cmd_string,commandstr);
			commandstr[0] = '\0';
			printf("Creating Command Thread\n");
			if (pthread_create(&command_thread, &attr, &command_switch, (void*) &sessioninfo) == -1) {
			        fprintf (stderr, "%s: pthread_create: %s\n", __FUNCTION__, strerror (errno));
				freeQueue(cmd_queue_ptr);
				return -1;
			}

		}
		//check if the command thread is done, if so, join
		else if (sessioninfo.cmd_complete) {
			printf("joining thread\n");
			if (pthread_join(command_thread,NULL) == -1) {
			        fprintf (stderr, "%s: pthread_join: %s\n", __FUNCTION__, strerror (errno));
				freeQueue(cmd_queue_ptr);
			        return -1;
			}
			command_thread = 0;
			sessioninfo.cmd_string[0] = '\0';
			sessioninfo.cmd_complete = false;
		}
		//add the command to the command queue
		//else
			//cmd_queue_ptr = addToQueue(commandstr, cmd_queue_ptr);
	}
	//if shutdown or quit was given, abort the current thread if running
	sessioninfo.cmd_abort = true;
	if (command_thread) {
	        if (pthread_join(command_thread,NULL) == -1) {
	                fprintf (stderr, "%s: pthread_join: %s\n", __FUNCTION__, strerror (errno));
			freeQueue(cmd_queue_ptr);
			return -1;
		}
	}

	//Close the data connection socket.
	if (sessioninfo.d_sfd > 0) {
	        if (close (sessioninfo.d_sfd) == -1)
	                fprintf (stderr, "%s: close: %s\n", __FUNCTION__, strerror (errno));
	}

	printf("session finished\n");
	freeQueue(cmd_queue_ptr);
	pthread_attr_destroy(&attr);
	return 0;

}


//Evan: I have changed the void return type to int, so that recv may return error.
int readCmd(char *str, int sock, session_info_t *si) {
	int rt = 0;
	int len = 0;

	//keep adding rxed chars to str until \n rxed
	while (1) {
	  if ((rt = recv(sock,str+len,1,0)) == -1) {
	        if (errno == EINTR)
		        continue;
		fprintf (stderr, "%s: recv: %s\n", __FUNCTION__, strerror (errno));
		return -1;
	  }
		if (rt > 0) {

			len += rt;
			if (str[len-1] == '\n') {

				str[len-1] = '\0'; //null terminate string
				break;
			}

		}
		else if (rt == 0) {
			si->cmd_abort = true;
			si->cmd_quit = true;
			break;
		}

	}

	return len;
}
