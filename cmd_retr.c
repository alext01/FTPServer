//=============================================================================
// Assignment #03 - FTP Server
//=============================================================================
// Date:        November 2013
// Course:      CMPT 361 - Introduction to Networks
// Instructor:  Dr. Nicholas M. Boers
// Students:    Evan Myers
//              Justin Slind
//              Alex Tai
//              James Yoo
//=============================================================================
// Filename:
//     cmd_retr.c
//=============================================================================
// Associated Header File(s):
//   > cmd_retr.h
//   > net.h
//   > session.h
//=============================================================================
// Brief Description:
//     
//     
//=============================================================================
// Code Citation(s):
//     * http://www.cplusplus.com/reference/
//     * http://www.stackoverflow.com/
//=============================================================================




//C Library Reference(s)
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>


//Header File Reference(s)
#include "cmd_retr.h"
#include "net.h"
#include "path.h"
#include "session.h"


//Preprocessor Macro Define(s)
#define BUFFSIZE 1000




//=============================================================================
// Function Name:
//   void command_retrieve(session_info_t *si, char *path)
//=============================================================================
// Brief Description:
//   
//=============================================================================
// List of Variables:
//   {noAccess}
//     > Type character pointer
//     > 
//   {noConnection}
//     > Type character pointer
//     > 
//=============================================================================
// Related Citation(s):
//   > N/A
//=============================================================================

void command_retrieve(session_info_t *si, char *path)
{ //BEGIN function 'command_retrieve'

  struct timeval timeout;

  fd_set wfds;

  FILE *retrFile;

  bool fileCheck;

  int retVal,
      selVal;

  char buffer[BUFFSIZE];

  char *aborted,
       *endLine,
       *fullPath,
       *middle,
       *noAccess,
       *noConnection,
       *success,
       *transferStart,
       *type;

  if ((si->logged_in == false) || (strcmp(si->user, "anonymous") == 0)) {
    noAccess = "550 - Access denied.\n";
    send_all(si->c_sfd, (uint8_t *)noAccess, strlen(noAccess));
    return;
  } //END statement 'if'

  if (si->d_sfd == 0) {
    noConnection = "425 - Cannot open data connection; please use the PORT or PASV command first.\n";
    send_all(si->c_sfd, (uint8_t *)noConnection, strlen(noConnection));
    return;
  } //END statement 'if'

  transferStart = "150 - File status okay; about to open data connection.\n";
  middle = " mode data connection for ";
  endLine = ".\n";
  send_all(si->c_sfd, (uint8_t *)transferStart, strlen(transferStart));

  if (si->type == 'a') {
    type = "ASCII";
  } else {
    type = "BINARY";
  } //END statement 'if-else'

  send_all(si->c_sfd, (uint8_t *)type, strlen(type));
  send_all(si->c_sfd, (uint8_t *)middle, strlen(middle));
  send_all(si->c_sfd, (uint8_t *)path, strlen(path));
  send_all(si->c_sfd, (uint8_t *)endLine, strlen(endLine));

  if (!(fileCheck = check_file_exist(si->cwd, path))) {
    return;
  } //END statement 'if'

  if ((fullPath = merge_paths(si->cwd, path, NULL)) == NULL) {
    return;
  } //END statement 'if'

  retrFile = fopen(fullPath, "w");
  free(fullPath);

  while ((si->cmd_abort == false) && (retVal != 0)) {
    FD_ZERO(&wfds);
    FD_SET(si->d_sfd, &wfds);
    timeout.tv_sec = 0;
    timeout.tv_usec = USEC_TIMEOUT;
    selVal = select((si->d_sfd + 1), NULL, &wfds, NULL, &timeout);

    if (selVal == -1) {

      if (errno == EINTR) {
	continue;
      } //END statement 'if'

      fprintf(stderr, "%s: select: %s\n", __FUNCTION__, strerror(errno));

    } else if (selVal == 0) {
      continue;
    } //END statement 'if-else'

    if (FD_ISSET(si->d_sfd, &wfds)) {

      if ((retVal = send(si->d_sfd, buffer, BUFFSIZE, 0)) > 0) {
	fwrite(buffer, sizeof(char), retVal, retrFile);
      } //END statement 'if'

    } //END statement 'if'

  } //END loop 'while'

  if (si->cmd_abort == true) {
    aborted = "426 - Connection close; transfer aborted.\n";
    send_all(si->c_sfd, (uint8_t *)aborted, strlen(aborted));
    si->cmd_abort = false;
  } else {
    success = "226 - Closing data connection; requested file action successful.\n";
    send_all(si->c_sfd, (uint8_t *)success, strlen(success));
  } //END statement 'if-else'

  fclose(retrFile);
  close(si->d_sfd);
  return;

} //END function 'command_retrieve'