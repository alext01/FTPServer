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
//   > cmd_switch.c
//=============================================================================
// Associated Header File(s):
//   > cmd_string_parser.h
//   > cmd_switch.h
//   > net.h
//   > session.h
//   > users.h
//=============================================================================
// Brief Description:
//
//
//
//=============================================================================
// Code Citation(s):
//   > http://www.cplusplus.com/reference/
//   > http://www.stackoverflow.com/
//=============================================================================




//C Library Reference(s)
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Header File Reference(s)
#include "cmd_string_parser.h"
#include "cmd_switch.h"
#include "net.h"
#include "session.h"
#include "users.h"
#include "cmd_stor.h"
#include "cmd_misc.h"


//Preprocessor Macro Define(s)
#define MIN_NUM_ARGS 1    //Minimum allowed number of arguments
#define MAX_CMD_SIZE 4    //Maximum allowed length of a command
#define MIN_CMD_SIZE 3    //Minimum allowed length of a command




//=============================================================================
// Function Name:
//   void *command_switch(void *param)
//=============================================================================
// Brief Description:
//     
//=============================================================================
// List of Variables:
//   {numArgs}
//     > Type integer
//     > 
//   {arg}
//     > Type character pointer
//     > 
//   {cmd}
//     > Type character pointer
//     > 
//   {cmdLine}
//     > Type character pointer
//     > 
//   {si}
//     > Type (session_info_t *)
//     > 
//=============================================================================
// Related Citation:
//   > N/A
//=============================================================================

void *command_switch(void *param)
{ //BEGIN function 'command_switch'

  int numArgs;

  char *arg,
       *cmd,
       *cmdLine;

  char *notimplemented = "502 Command not Implemented.\n";

  session_info_t *si;

  si = (session_info_t *)param;
  cmdLine = si->cmd_string;
  numArgs = command_arg_count(cmdLine);

  if (numArgs >= MIN_NUM_ARGS) {

    cmd = command_extract_cmd(cmdLine);
    arg = command_extract_arg(cmdLine);

    //Debug Print
    printf("CHECK: Return value of 'cmd' <%s> and 'arg' \"%s\"\n", cmd, arg);

    //=========================================================================
    // Brief Description:
    //   The following block of the 'if-else' statement will execute if the
    //   given command has a length of four.
    //=========================================================================
    //                            BEGIN FIRST BLOCK
    //=========================================================================

    if (strlen(cmd) == MAX_CMD_SIZE) {

      //=======================================================================
      //        MINIMUM IMPLEMENTATION/FREQUENTLY USED COMMANDS >BEGIN<
      //=======================================================================

      /* USER <SP> <username> <CRLF> */
      if (strcmp(cmd, "USER") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command USER Invoked
	cmd_user(si, arg);

      /* PASS <SP> <password> <CRLF> */
      } else if (strcmp(cmd, "PASS") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command PASS Invoked
	cmd_pass(si, arg);

      /* QUIT <CRLF> */
      } else if (strcmp(cmd, "QUIT") == 0) {



	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command QUIT Invoked
	  char *goodbye = "221 Goodbye.\n";
	  send_all(si->c_sfd,(uint8_t*)goodbye,strlen(goodbye));
	  si->cmd_quit = true;

      /* PORT <SP> <host-port> <CRLF> */
      } else if (strcmp(cmd, "PORT") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command PORT Invoked
	cmd_port(si, arg);

      /* PASV <CRLF> */
      } else if (strcmp(cmd, "PASV") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

	//Command PASV Invoked
	cmd_pasv(si);

      /* TYPE <SP> <type-code> <CRLF> */
      } else if (strcmp(cmd, "TYPE") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);
	cmd_type(si, arg);

      /* STRU <SP> <structure-code> <CRLF> */
      } else if (strcmp(cmd, "STRU") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* MODE <SP> <mode-code> <CRLF> */
      } else if (strcmp(cmd, "MODE") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* RETR <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RETR") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* STOR <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "STOR") == 0) {
    	  cmd_stor(si,arg);
	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* LIST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(cmd, "LIST") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* HELP [<SP> <string>] <CRLF> */
      } else if (strcmp(cmd, "HELP") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* NOOP <CRLF> */
      } else if (strcmp(cmd, "NOOP") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      //=======================================================================
      //         MINIMUM IMPLEMENTATION/FREQUENTLY USED COMMANDS >END<
      //=======================================================================
      
      /* ACCT <SP> <account-information> <CRLF> */
      } else if (strcmp(cmd, "ACCT") == 0) {

	//Debug Print
        printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* CDUP <CRLF> */
      } else if (strcmp(cmd, "CDUP") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* SMNT <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "SMNT") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* REIN <CRLF> */
      } else if (strcmp(cmd, "REIN") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* STOU <CRLF> */
      } else if (strcmp(cmd, "STOU") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* APPE <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "APPE") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);
		cmd_appe(si,arg);

      /* ALLO <SP> <decimal-integer> [<SP> R <SP> <decimal-integer>]<CRLF> */
      } else if (strcmp(cmd, "ALLO") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* REST <SP> <marker> <CRLF> */
      } else if (strcmp(cmd, "REST") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* RNFR <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RNFR") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* RNTO <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RNTO") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* ABOR <CRLF> */
      } else if (strcmp(cmd, "ABOR") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* DELE <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "DELE") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* NLIST [<SP> <pathname>] <CRLF> */
      } else if (strcmp(cmd, "NLST") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* SITE <SP> <string> <CRLF> */
      } else if (strcmp(cmd, "SITE") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* SYST <CRLF> */
      } else if (strcmp(cmd, "SYST") == 0) {

	//Debug Print
    	  printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);
    	  send_all(si->c_sfd,(uint8_t*)notimplemented,strlen(notimplemented));

      /* STAT [<SP> <pathname>] <CRLF> */
      } else if (strcmp(cmd, "STAT") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else {

	//Debug Print
	printf("ERROR: Command <%s> Unknown!\n", cmd);
	printf("Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

      } //END statement 'if-else'

    //=========================================================================
    //                             END FIRST BLOCK
    //=========================================================================
    // Brief Description:
    //   The following block of the 'if-else' statement will execute if the
    //   given command has a length of three.
    //=========================================================================
    //                            BEGIN SECOND BLOCK
    //=========================================================================

    } else if (strlen(cmd) == MIN_CMD_SIZE) {

      /* CWD <SP> <pathname> <CRLF> */
      if (strcmp(cmd, "CWD") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* RMD <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "RMD") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* MKD <SP> <pathname> <CRLF> */
      } else if (strcmp(cmd, "MKD") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* PWD <CRLF> */
      } else if (strcmp(cmd, "PWD") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else {

	//Debug Print
	printf("ERROR: Command <%s> Unknown!\n", cmd);
	printf("Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

      } //END statement 'if-else'

    //=========================================================================
    //                             END SECOND BLOCK
    //=========================================================================

    } else {

      //Debug Print
      printf("ERROR: Command <%s> Unknown!\n", cmd);
      printf("Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

    } //END statement 'if-else'

    free(cmd);
    free(arg);

  } else {

    //Debug Print
    printf("ERROR: Missing Command/Insufficient Arguments!\n");
    printf("Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdLine));

  } //END statement 'if-else'

  si->cmd_complete = true;
  return NULL;

} //END function 'command_switch'
