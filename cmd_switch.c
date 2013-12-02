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
//    > cmd_switch.c
//===============================================================================
//  Associated Header File(s):
//    > cmd_help.h
//    > cmd_misc.h
//    > cmd_retr.h
//    > cmd_stor.h
//    > cmd_string_parser.h
//    > cmd_switch.h
//    > filemanip.h
//    > net.h
//    > session.h
//    > users.h
//===============================================================================
//  Overview:
//    A massive 'if-else if' statement that determines the command that has been
//    invoked by the client and performs an appropriately related action.
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//    > http://www.stackoverflow.com/
//===============================================================================




//C Library Reference(s)
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Header File Reference(s)
#include "cmd_help.h"
#include "cmd_misc.h"
#include "cmd_retr.h"
#include "cmd_stor.h"
#include "cmd_string_parser.h"
#include "cmd_switch.h"
#include "filemanip.h"
#include "net.h"
#include "session.h"
#include "users.h"


//Preprocessor Macro Define(s)
#define MAX_CMD_SIZE 4    //Maximum allowed length of a command
#define MIN_CMD_SIZE 3    //Minimum allowed length of a command
#define MIN_NUM_ARGS 1    //Minimum allowed number of arguments




//===============================================================================
//  Function Name:
//    command_switch(void *param)
//===============================================================================
//  Description:
//    Consult file "cmd_switch.h"
//===============================================================================
//  Variable Listing (in alphabetical order):
//    {arg}
//      > Type character pointer
//      > Contains the entire argument string (excludes the command)
//    {argCount}
//      > Type integer
//      > Contains the number of arguments passed from the command string (with
//        the command itself being excluded from the count)
//    {cmd}
//      > Type character pointer
//      > Contains the invoked command (excludes the argument string)
//    {cmdLine}
//      > Type character pointer
//      > Contains the entirety of the command string entered by the client
//    {cmdUnimplemented}
//      > Type character pointer
//      > Contains the FTP server return code 502
//    {cmdUnrecognized}
//      > Type character pointer
//      > Contains the FTP server return code 501
//    {numArgs}
//      > Type integer
//      > Contains the number of arguments passed from the command string (with
//        the command itself being included in the count)
//    {printEnd}
//      > Type character pointer
//      > Contains part of the FTP server return code 257
//    {printStart}
//      > Type character pointer
//      > Contains part of the FTP server retrun code 257
//    {retCodeQuit}
//      > Type character pointer
//      > Contains the FTP server return code 221
//    {si}
//      > Type structure pointer
//      > Contains all pertinent information in regards to the current client
//===============================================================================
//  Related Citation:
//    > N/A
//===============================================================================

void *command_switch(void *param)
{ //BEGIN function 'command_switch'

  session_info_t *si;

  int argCount,
      numArgs;

  char *arg,
       *cmd,
       *cmdLine;

  char *cmdUnimplemented,
       *cmdUnrecognized;

  si = (session_info_t *)param;
  cmdLine = si->cmd_string;
  numArgs = command_arg_count(cmdLine);
  argCount = 0;

  cmdUnrecognized = "500 - Syntax error, command unrecognized.\n";
  cmdUnimplemented = "502 - Command is not currently implemented.\n";

  if (numArgs >= MIN_NUM_ARGS) {

    cmd = command_extract_cmd(cmdLine);
    arg = command_extract_arg(cmdLine);

    //Debug Print
    fprintf(stderr, "CHECK: Return value of 'cmd' <%s> and 'arg' \"%s\"\n", cmd, arg);

    //===========================================================================
    //  Brief Description:
    //    The following block of the 'if-else' statement will execute if the
    //    given command has a length of four.
    //===========================================================================
    //                             BEGIN FIRST BLOCK
    //===========================================================================

    if (strlen(cmd) == MAX_CMD_SIZE) {

      //=========================================================================
      //         MINIMUM IMPLEMENTATION/FREQUENTLY USED COMMANDS >BEGIN<
      //=========================================================================

      /* USER <SP> <username> <CRLF> */
      if (strcmp(cmd, "USER") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command USER Invoked
	cmd_user(si, arg);

      /* PASS <SP> <password> <CRLF> */
      } else if (strcmp(cmd, "PASS") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command PASS Invoked
	cmd_pass(si, arg);

      /* QUIT <CRLF> */
      } else if (strcmp(cmd, "QUIT") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command QUIT Invoked
	char *retCodeQuit;

	retCodeQuit = "221 - Quitting system; goodbye.\n";
	send_all(si->c_sfd, (uint8_t *)retCodeQuit, strlen(retCodeQuit));
	si->cmd_quit = true;

      /* PORT <SP> <host-port> <CRLF> */
      } else if (strcmp(cmd, "PORT") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command PORT Invoked
	cmd_port(si, arg);

      /* PASV <CRLF> */
      } else if (strcmp(cmd, "PASV") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command PASV Invoked
	cmd_pasv(si);

      /* TYPE <SP> <type-code> <CRLF> */
      } else if (strcmp(cmd, "TYPE") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command TYPE Invoked
	cmd_type(si, arg);

      /* STRU <SP> <structure-code> <CRLF> */
      } else if (strcmp(cmd, "STRU") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command STRU Invoked
	if (arg != NULL) {
	  argCount = command_arg_count(arg);
	  convert_to_upper(arg);
	} //END statement 'if'

	cmd_stru(si, arg, argCount);

      /* MODE <SP> <mode-code> <CRLF> */
      } else if (strcmp(cmd, "MODE") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command MODE Invoked
    	cmd_mode(si, arg);

      /* RETR <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RETR") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command RETR Invoked
    	command_retrieve(si, arg);

      /* STOR <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "STOR") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command STOR Invoked
    	cmd_stor(si, arg);

      /* STOU <CRLF> */
      } else if (strcmp(cmd, "STOU") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	//Command STOU Invoked
	cmd_stou(si, arg);

      /* APPE <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "APPE") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command APPE Invoked
	cmd_appe(si, arg);

      /* LIST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(cmd, "LIST") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command NLST Invoked
	cmd_list_nlst(si, arg, true);

      /* NLST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(cmd, "NLST") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	//Command NLST Invoked
	cmd_list_nlst(si, arg, false);

      /* SYST <CRLF> */
      } else if (strcmp(cmd, "SYST") == 0) {

	//Debug Print
    	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command SYST Invoked
	cmd_syst(si);

      /* HELP [<SP> <string>] <CRLF> */
      } else if (strcmp(cmd, "HELP") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command HELP Invoked
	if (arg != NULL) {
	  convert_to_upper(arg);
	} //END statement 'if'

	command_help(si, arg);

      /* NOOP <CRLF> */
      } else if (strcmp(cmd, "NOOP") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      //=========================================================================
      //          MINIMUM IMPLEMENTATION/FREQUENTLY USED COMMANDS >END<
      //=========================================================================
      
      /* ACCT <SP> <account-information> <CRLF> */
      } else if (strcmp(cmd, "ACCT") == 0) {

	//Debug Print
        fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* CDUP <CRLF> */
      } else if (strcmp(cmd, "CDUP") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command CDUP Invoked
	cmd_cdup (si, arg);

      /* SMNT <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "SMNT") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* REIN <CRLF> */
      } else if (strcmp(cmd, "REIN") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* ALLO <SP> <decimal-integer> [<SP> R <SP> <decimal-integer>] <CRLF> */
      } else if (strcmp(cmd, "ALLO") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* REST <SP> <marker> <CRLF> */
      } else if (strcmp(cmd, "REST") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* RNFR <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RNFR") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* RNTO <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RNTO") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* ABOR <CRLF> */
      } else if (strcmp(cmd, "ABOR") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* DELE <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "DELE") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* SITE <SP> <string> <CRLF> */
      } else if (strcmp(cmd, "SITE") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* STAT [<SP> <pathname>] <CRLF> */
      } else if (strcmp(cmd, "STAT") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      } else {

	//Debug Print
	fprintf(stderr, "ERROR: Command <%s> Unknown!\n", cmd);
	fprintf(stderr, "Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      } //END statement 'if-else if'

    //===========================================================================
    //                              END FIRST BLOCK
    //===========================================================================
    //  Brief Description:
    //    The following block of the 'if-else' statement will execute if the
    //    given command has a length of three.
    //===========================================================================
    //                             BEGIN SECOND BLOCK
    //===========================================================================

    } else if (strlen(cmd) == MIN_CMD_SIZE) {

      /* CWD <SP> <pathname> <CRLF> */
      if (strcmp(cmd, "CWD") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command CWD Invoked
	cmd_cwd (si, arg);

      /* RMD <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RMD") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      /* MKD <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "MKD") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

    	//Command MKD Invoked
	makeDir(si, arg);

      /* PWD <CRLF> */
      } else if (strcmp(cmd, "PWD") == 0) {

	//Debug Print
	fprintf(stderr, "Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	char *printEnd,
	     *printStart;

	printStart = "257 - \"";
	printEnd = "\".\n";
	
	send_all(si->c_sfd, (uint8_t *)printStart, strlen(printStart));
	send_all(si->c_sfd, (uint8_t *)si->cwd, strlen(si->cwd));
	send_all(si->c_sfd, (uint8_t *)printEnd, strlen(printEnd));

      } else {

	//Debug Print
	fprintf(stderr, "ERROR: Command <%s> Unknown!\n", cmd);
	fprintf(stderr, "Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

	send_all(si->c_sfd, (uint8_t *)cmdUnimplemented, strlen(cmdUnimplemented));

      } //END statement 'if-else'

    //===========================================================================
    //                              END SECOND BLOCK
    //===========================================================================

    } else {

      //Debug Print
      fprintf(stderr, "ERROR: Command <%s> Unknown!\n", cmd);
      fprintf(stderr, "Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

      send_all(si->c_sfd, (uint8_t *)cmdUnrecognized, strlen(cmdUnrecognized));

    } //END statement 'if-else'

    free(cmd);
    free(arg);

  } else {

    //Debug Print
    fprintf(stderr, "ERROR: Missing Command/Insufficient Arguments!\n");
    fprintf(stderr, "Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

  } //END statement 'if-else'

  si->cmd_complete = true;
  return NULL;

} //END function 'command_switch'
