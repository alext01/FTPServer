//===============================================================================
//  Assignment #03 - FTP Server
//===============================================================================
//  Date:         November 2013
//  Course:       CMPT 361 - Introduction to Networks
//  Instructor:   Dr. Nicholas M. Boers
//  Students:     Evan Myers
//                Justin Slind
//                Alex Tai
//                James Yoo
//===============================================================================
//  Filename:
//    cmd_retr.c
//===============================================================================
//  Associated Header File(s):
//    > cmd_retr.h
//    > net.h
//    > path.h
//    > response.h
//    > session.h
//===============================================================================
//  Overview:
//    
//    
//    
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//    > http://www.stackoverflow.com/
//===============================================================================




//C Library Reference(s)
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//Header File Reference(s)
#include "cmd_retr.h"
#include "net.h"
#include "path.h"
#include "response.h"
#include "session.h"


//Preprocessor Macro Define(s)
#define BUFFSIZE 1000




//===============================================================================
//  Function Name:
//    command_retrieve(session_info_t *si, char *path)
//===============================================================================
//  Description:
//    Consult file "cmd_retr.h"
//===============================================================================
//  Variable Listing (in alphabetical order):
//    {noAccess}
//      > Type character pointer
//      > 
//    {noConnection}
//      > Type character pointer
//      > 
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

void command_retrieve(session_info_t *si, char *path)
{ //BEGIN function 'command_retrieve'

  struct timeval timeout;

  fd_set wfds;

  FILE *retrFile;

  bool fileCheck;

  int retVal, //Evans note: needs to be initialized, not 0 or uninit.
      selVal;

  char buffer[BUFFSIZE];

  char *aborted,
       *fullPath,
       *noAccess,
       *noConnection,
       *success,
       *transferStart;

  if (si->logged_in == false) {
    noAccess = "550 - Access denied.\n";
    send_all(si->c_sfd, (uint8_t *)noAccess, strlen(noAccess));
    return;
  } //END statement 'if'

  if (!(fileCheck = check_file_exist(si->cwd, path))) {
    send_mesg_553 (si->c_sfd);
    return;
  } //END statement 'if'

  transferStart = "150 - File status okay; about to open data connection.\n";
  send_all(si->c_sfd, (uint8_t *)transferStart, strlen(transferStart));

  if (si->d_sfd == 0) {
    noConnection = "425 - Cannot open data connection; please use the PORT or PASV command first.\n";
    send_all(si->c_sfd, (uint8_t *)noConnection, strlen(noConnection));
    return;
  } //END statement 'if'

  if ((fullPath = merge_paths(si->cwd, path, NULL)) == NULL) {
    send_mesg_451 (si->c_sfd);
    close(si->d_sfd);
    si->d_sfd = 0;
    return;
  } //END statement 'if'

  if ((retrFile = fopen(fullPath, "r")) == NULL) {
    fprintf (stderr, "%s: fopen: %s\n", __FUNCTION__, strerror (errno));
    free(fullPath);
    send_mesg_451 (si->c_sfd);
    close (si->d_sfd);
    si->d_sfd = 0;
  }

  free(fullPath);
  retVal = BUFFSIZE;

  while ((si->cmd_abort == false) && (retVal != 0)) {
    FD_ZERO(&wfds);
    FD_SET(si->d_sfd, &wfds);
    timeout.tv_sec = COM_THREAD_ABORT_TIMEOUT_SEC;
    timeout.tv_usec = COM_THREAD_ABORT_TIMEOUT_USEC;
    selVal = select((si->d_sfd + 1), NULL, &wfds, NULL, &timeout);


    if (selVal == -1) {

      if (errno == EINTR) {
	continue;
      } //END statement 'if'

      fprintf(stderr, "%s: select: %s\n", __FUNCTION__, strerror(errno));
      send_mesg_451(si->c_sfd);
      close(si->d_sfd);
      si->d_sfd = 0;
      return;
    } else if (selVal == 0) {
      continue;
    } //END statement 'if-else'

    if (FD_ISSET(si->d_sfd, &wfds)) {

      if ((retVal = fread(buffer, sizeof(*buffer), BUFFSIZE, retrFile)) == 0) {
	
	if (ferror (retrFile)) {
	  fprintf (stderr, "%s: fread: error while processing\n", __FUNCTION__);
	  send_mesg_451(si->c_sfd);
	  close(si->d_sfd);
	  si->d_sfd = 0;
	  return;
	} else if (feof(retrFile)) {
	  break;
	} //END statement 'if-else'

      } //END statement 'if'

      if (send_all(si->d_sfd, (uint8_t *)buffer, retVal) == -1) {
	send_mesg_451(si->c_sfd);
	close(si->d_sfd);
	si->d_sfd = 0;
	return;
      } //END statement 'if'

    } //END statement 'if'

  } //END loop 'while'


  if (fclose(retrFile) == EOF) {
    fprintf (stderr, "%s: fclose: %s\n", __FUNCTION__, strerror (errno));
  } //END statement 'if'
 
  if (close(si->d_sfd) == -1) {
    fprintf (stderr, "%s: close: %s\n", __FUNCTION__, strerror (errno));
  } //END statement 'if'
  
  si->d_sfd = 0;

  if (si->cmd_abort == true) {
    aborted = "426 - Connection close; transfer aborted.\n";
    send_all(si->c_sfd, (uint8_t *)aborted, strlen(aborted));
    si->cmd_abort = false;
  } else {
    success = "226 - Closing data connection; requested file action successful.\n";
    send_all(si->c_sfd, (uint8_t *)success, strlen(success));
  } //END statement 'if-else'

  return;

} //END function 'command_retrieve'
