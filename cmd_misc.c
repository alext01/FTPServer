/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: cmd_misc.c
 * Date: November 2013
 *
 * Description:
 *   Miscellaneous ftp commands that don't fit in other categories
 *****************************************************************************/

#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "cmd_misc.h"
#include "session.h"
#include "net.h"


void cmd_type(session_info_t *si, char *arg) {

  char *fail = "504 Command not implemented for that parameter.\n";
  char *ascii = "200 Switching to ASCII mode.\n";
  //must log in to change type
  if (!si->logged_in) {
    char *notloggedin = "530 Please login with USER and PASS.\n";
    send_all(si->c_sfd,(uint8_t*)notloggedin,strlen(notloggedin));
    return;
  }

  if (!arg) {
    char *syntaxerror = "501 Syntax error in arguments.\n";
    send_all(si->c_sfd, (uint8_t*)syntaxerror, strlen(syntaxerror));
    return;
  }

  // if arg is 1 char it must be an a or i, case insensitive
  if (strlen(arg) == 1) {
    arg[0] = tolower(arg[0]); //change arg to lowercase
    if (arg[0] == 'a') {

      send_all(si->c_sfd,(uint8_t*)ascii,strlen(ascii));
      si->type = 'a';
    } else if (arg[0] == 'i') {
      char *image = "200 Switching to Binary mode.\n";
      send_all(si->c_sfd,(uint8_t*)image,strlen(image));
      si->type = 'i';
    } else {
      send_all(si->c_sfd,(uint8_t*)fail,strlen(fail));
    }
    return;
  } if (strlen(arg) == 3) {
    //set interesting chars to lowercase
    if (strcasecmp(arg,"a n") == 0) {
      send_all(si->c_sfd,(uint8_t*)ascii,strlen(ascii));
      si->type ='a';
      return;
    }
  }
  send_all(si->c_sfd,(uint8_t*)fail,strlen(fail));
  return;
}

void cmd_mode(session_info_t *si, char *arg) {
  char *fail = "504 Command not implemented for that parameter.\n";
  char *ascii = "200 Switching to stream mode.\n";
  if (arg)
    if (strlen(arg) == 1) {
      arg[0] = tolower(arg[0]); //change arg to lowercase
      if (arg[0] == 's') {

	send_all(si->c_sfd,(uint8_t*)ascii,strlen(ascii));
	return;

      }
    }
  send_all(si->c_sfd,(uint8_t*)fail,strlen(fail));
  return;
}

//Send the system type on the control connection.
void cmd_syst (session_info_t *si) {
  char *system = "215 UNIX Type: L8\n";
  send_all (si->c_sfd, (uint8_t*)system, strlen (system));
}

//===============================================================================
//  Function Name:
//    cmd_stru(session_info_t *si, char *arg, int argCount)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Variable List (in alphabetic order):
//    {argUnimplemented}
//      > Type character pointer
//      > Contains the FTP server return code 504
//    {argUnrecognized}
//      > Type character pointer
//      > Contains the FTP server return code 501
//    {fileStructure}
//      > Type character pointer
//      > Contains the FTP server return code 200
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================
void cmd_stru(session_info_t *si, char *arg, int argCount)
{ //BEGIN function 'cmd_stru'

  char *fileStructure = "200 - Switching to File Structure.\n",
       *argUnrecognized = "501 - Syntax error in parameters; incorrect number of parameters.\n",
       *argUnimplemented = "504 - Command is not implemented for that parameter.\n";

  if ((argCount > MAX_NUM_ARGS) || (arg == NULL)) {
    send_all(si->c_sfd, (uint8_t *)argUnrecognized, strlen(argUnrecognized));
    return;
  } //END statement 'if'

  if (strcmp(arg, "F") == 0) {
    send_all(si->c_sfd, (uint8_t *)fileStructure, strlen(fileStructure));
    return;
  } else {
    send_all(si->c_sfd, (uint8_t *)argUnimplemented, strlen(argUnimplemented));
    return;
  } //END statement 'if-else'

} //END function 'cmd_stru'
