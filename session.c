#include <pthread.h>
#include <stdbool.h>
#include <sys/select.h>
#include <string.h>
#include "session.h"



extern int shutdown;

int session(int c_sfd) {   //queue cmd_queue_ptr will need to be sent too

	pthread_t command_thread = 0;
	session_info_t sessioninfo;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	char commandstr[CMD_STRLEN];
	struct timeval timeout;
	fd_set rfds, wfds;

	//init sessioninfo
	sessioninfo.c_sfd = c_sfd;
	sessioninfo.d_sfd = 0;
	sessioninfo.cmd_abort = false;
	sessioninfo.cmd_quit = false;
	sessioninfo.logged_in = false;
	sessioninfo.cmd_complete = false;
	sessioninfo.user[0] = '\0';
	sessioninfo.cwd[0] = '\0';
	sessioninfo.cmd_string[0] = '\0';


	//check if the server is shutting down or if the quit cmd was given
	while (!shutdown && !sessioninfo.cmd_quit) {

		FD_ZERO(&rfds);
		FD_SET(c_sfd,&rfds);
		timeout.tv_sec = 0;
		timeout.tv_usec = USEC_TIMEOUT;


		select(c_sfd+1,&rfds,NULL,NULL,&timeout);  //read from socket w/ timeout

		//if there's anything to read on the control socket, do so.
		if (FD_ISSET(c_sfd, &rfds))
			readCmd(commandstr, c_sfd);

		//if command is ABORT let the current thread know
		if (strncmp(commandstr,"ABRO",4))
			sessioninfo.cmd_abort = true;

		//if there isn't a command_thread already, create one to either handle a command
		//on the command_queue or the current command
		else if (!command_thread) {
			if (cmd_queue_ptr) {
				addToQueue(commandstr, cmd_queue_ptr);
				pullFromQueue(commandstr, cmd_queue_ptr);
			}
			strcpy(sessioninfo.cmd_string,commandstr);
			pthread_create(&command_thread, &attr, &command_switch, (void*) &sessioninfo);

		}
		//check if the command thread is done, if so, join
		else if (sessioninfo.cmd_complete) {
			pthread_join(command_thread,NULL);
			command_thread = 0;
			sessioninfo.cmd_string[0] = '\0';
			sessioninfo.cmd_complete = false;
		}
		//add the command to the command queue
		else
			addToQueue(commandstr, cmd_queue_ptr);
	}
	return 0;

}
