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
//    cmd_misc.c
//===============================================================================
//  Associated Header File(s):
//    > cmd_misc.h
//    > net.h
//    > session.h
//===============================================================================
//  Overview:
//    This file implements various miscellaneous FTP server commands that do not
//    fit into other categories.
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//    > http://www.stackoverflow.com/
//===============================================================================




//C Library Reference(s)
#include <strings.h>
#include <string.h>
#include <ctype.h>


//Header File Reference(s)
#include "cmd_misc.h"
#include "net.h"
#include "session.h"




//===============================================================================
//  Function Name:
//    cmd_type(session_info_t *si, char *arg)
//===============================================================================
//  Description:
//    Consult file "cmd_misc.h"
//===============================================================================
//  Variable Listing (in alphabetical order):
//    {ascii}
//      > Type character pointer
//      > Contains the FTP server return code 200
//    {fail}
//      > Type character pointer
//      > Contains the FTP server return code 504
//    {image}
//      > Type character pointer
//      > Contains the FTP server return code 200
//    {notloggedin}
//      > Type character pointer
//      > Contains the FTP server return code 530
//    {syntaxerror}
//      > Type character pointer
//      > Contains the FTP server return code 501
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

void cmd_type(session_info_t *si, char *arg)
{ //BEGIN function 'cmd_type'

  char *ascii = "200 Switching to ASCII mode.\n";
  char *fail = "504 Command not implemented for that parameter.\n";

  //must log in to change type
  if (!si->logged_in) {
    char *notloggedin = "530 Please login with USER and PASS.\n";
    send_all(si->c_sfd, (uint8_t *)notloggedin, strlen(notloggedin));
    return;
  } //END statement 'if'

  if (!arg) {
    char *syntaxerror = "501 Syntax error in arguments.\n";
    send_all(si->c_sfd, (uint8_t *)syntaxerror, strlen(syntaxerror));
    return;
  } //END statement 'if'

  // if arg is 1 char it must be an a or i, case insensitive
  if (strlen(arg) == 1) {
    arg[0] = tolower(arg[0]); //change arg to lowercase

    if (arg[0] == 'a') {
      send_all(si->c_sfd, (uint8_t *)ascii, strlen(ascii));
      si->type = 'a';
    } else if (arg[0] == 'i') {
      char *image = "200 Switching to Image mode.\n";
      send_all(si->c_sfd, (uint8_t *)image, strlen(image));
      si->type = 'i';
    } else {
      send_all(si->c_sfd, (uint8_t *)fail, strlen(fail));
    } //END statement 'if-else'

    return;

  } //END statement 'if'

  if (strlen(arg) == 3) {
    
    //set interesting chars to lowercase
    if (strcasecmp(arg, "a n") == 0) {
      send_all(si->c_sfd, (uint8_t *)ascii, strlen(ascii));
      si->type ='a';
      return;
    } //END statement 'if'

  } //END statement 'if'

  send_all(si->c_sfd, (uint8_t *)fail, strlen(fail));

  return;

} //END function 'cmd_type'




//===============================================================================
//  Function Name:
//    cmd_mode(session_info_t *si, char *arg)
//===============================================================================
//  Description:
//    Consult file "cmd_misc.h"
//===============================================================================
//  Variable Listing (in alphabetical order):
//    {ascii}
//      > Type character pointer
//      > Contains the FTP server return code 200
//    {fail}
//      > Type character pointer
//      > Contains the FTP server return code 504
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

void cmd_mode(session_info_t *si, char *arg)
{ //BEGIN function 'cmd_mode'

  char *ascii = "200 Switching to stream mode.\n";
  char *fail = "504 Command not implemented for that parameter.\n";

  if (arg)

    if (strlen(arg) == 1) {
      arg[0] = tolower(arg[0]); //change arg to lowercase

      if (arg[0] == 's') {
	send_all(si->c_sfd, (uint8_t *)ascii, strlen(ascii));
	return;	
      } //END statement 'if'

    } //END statement 'if'

  send_all(si->c_sfd, (uint8_t *)fail, strlen(fail));
  return;

} //END function 'cmd_mode'




//===============================================================================
//  Function Name:
//    cmd_syst(session_info_t *si)
//===============================================================================
//  Description:
//    Consult file "cmd_misc.h"
//===============================================================================
//  Variable Listing (in alphabetical order):
//    {system}
//      > Type character pointer
//      > Contains the FTP server return code 215
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

//Send the system type on the control connection.
void cmd_syst(session_info_t *si)
{ //BEGIN function 'cmd_syst'

  char *system = "215 UNIX Type: L8\n";

  send_all(si->c_sfd, (uint8_t *)system, strlen(system));

} //END function 'cmd_syst'




//===============================================================================
//  Function Name:
//    cmd_stru(session_info_t *si, char *arg, int argCount)
//===============================================================================
//  Description:
//    Consult file "cmd_misc.h"
//===============================================================================
//  Variable Listing (in alphabetical order):
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
