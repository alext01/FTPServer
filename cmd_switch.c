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


//=============================================================================
// Filename:
//     cmd_switch.c
//=============================================================================
// Brief Description:
//
//
//
//=============================================================================


//=============================================================================
// Code Citation(s):
//     * http://www.cplusplus.com/reference/
//     * http://www.stackoverflow.com/
//=============================================================================


//C Library References
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Header File References
#include "cmd_line_parser.h"
#include "cmd_switch.h"
#include "net.h"
#include "session.h"


//Proprocessor Macro Defines
#define MIN_ARGS 1
#define MAX_CMD_SIZE 4
#define MIN_CMD_SIZE 3



//=============================================================================
// Brief Description:
//     
//=============================================================================
void *command_switch(void *param)
{ //BEGIN function 'command_switch'

  int numArgs;

  char *arg,
       *cmd,
       *cmdLine;

  session_info_t *si;

  si = (session_info_t *)param;
  cmdLine = si->cmd_string;
  numArgs = command_arg_count(cmdLine);

  if (numArgs >= MIN_ARGS) {

    cmd = command_extract_cmd(cmdLine);
    arg = command_extract_arg(cmdLine);

    //=========================================================================
    // Brief Description:
    //     The following block of the 'if-else' statement will execute if the
    //     given command has a length of four.
    //=========================================================================
    //                            BEGIN FIRST BLOCK
    //=========================================================================

    if (strlen(cmd) == MAX_CMD_SIZE) {

      //=======================================================================
      //            MINIMUM IMPLEMENTATION/FREQUENTLY USED COMMANDS
      //=======================================================================

      /* USER <SP> <username> <CRLF> */
      if (strcmp(cmd, "USER") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* PASS <SP> <password> <CRLF> */
      } else if (strcmp(cmd, "PASS") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* QUIT <CRLF> */
      } else if (strcmp(cmd, "QUIT") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      /* PORT <SP> <host-port> <CRLF> */
      } else if (strcmp(cmd, "PORT") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);
	cmd_port(si, arg);

      /* PASV <CRLF> */
      } else if (strcmp(cmd, "PASV") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);
	cmd_pasv(si, arg);

      /* TYPE <SP> <type-code> <CRLF> */
      } else if (strcmp(cmd, "TYPE") == 0) {

	//Debug Print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

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

      }

      //=======================================================================
      //          MINIMUM IMPLEMENTATION/FREQUENTLY USED COMMANDS END
      //=======================================================================
      
      else if (strcmp(cmd, "ACCT") == 0) {

	//Test print
        printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "CDUP") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "SMNT") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "REIN") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "STOU") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "APPE") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "ALLO") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "REST") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "RNFR") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "RNTO") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "ABOR") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "DELE") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "NLST") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "SITE") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "SYST") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "STAT") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else {

	printf("ERROR: Command <%s> Unknown!\n", cmd);
	printf("Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdString));

      } //END statement 'if-else'

    //=========================================================================
    //                             END FIRST BLOCK
    //=========================================================================
    // Brief Description:
    //     The following block of the 'if-else' statement will execute if the
    //     given command has a length of three.
    //=========================================================================
    //                            BEGIN SECOND BLOCK
    //=========================================================================

    } else if (strlen(cmd) == MIN_CMD_SIZE) {

      if (strcmp(cmd, "CWD") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "RMD") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "MKD") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else if (strcmp(cmd, "PWD") == 0) {

	//Test print
	printf("Invoked Command <%s> with (%d) Argument(s) \"%s\"\n", cmd, (numArgs - 1), arg);

      } else {

	printf("ERROR: Command <%s> Unknown!\n", cmd);
	printf("Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdString));

      } //END statement 'if-else'

    } //END statement 'if-else'

    free(cmd);
    free(arg);

    //=========================================================================
    //                             END SECOND BLOCK
    //=========================================================================

  } else {

    printf("ERROR: Missing Command/Insufficient Arguments!\n");
    printf("Argument Count (%d)\nString Length (%d)\n", numArgs, (int)strlen(cmdString));

  } //END statement 'if-else'

  si->cmd_complete = true;
  return NULL;

} //END function 'command_switch'
