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


//C Library References
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Header File References
#include "cmd_line_parser.h"
#include "cmd_switch.h"
#include "session.h"
#include "net.h"


//Proprocessor Macro Defines
#define MIN_ARGS 1
#define MAX_CMD_SIZE 4
#define MIN_CMD_SIZE 3



//=============================================================================
// Brief Description:
//     
//=============================================================================
void command_switch(void *param)
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

      if (strcmp(cmd, "USER") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "PASS") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "ACCT") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "CDUP") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "SMNT") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "QUIT") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "REIN") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "PORT") == 0) {

	cmd_port(si, arg);
	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "PASV") == 0) {

	cmd_pasv(si, arg);
	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "TYPE") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "STRU") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "MODE") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "RETR") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "STOR") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "STOU") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "APPE") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "ALLO") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "REST") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "RNFR") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "RNTO") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "ABOR") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "DELE") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "LIST") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "NLST") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "SITE") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "SYST") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "STAT") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "HELP") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "NOOP") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else {

	printf("ERROR: Command does not exist!\n");

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
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "RMD") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "MKD") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else if (strcmp(cmd, "PWD") == 0) {

	//Test print
	printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

      } else {

	printf("ERROR: Command does not exist!\n");

      } //END statement 'if-else'

    } //END statement 'if-else'
    //=========================================================================
    //                             END SECOND BLOCK
    //=========================================================================

  } else {

    printf("ERROR: Incorrect Command/Insufficient Arguments!\n");

  } //END statement 'if-else'

  free(cmd);
  free(arg);
  si->cmd_complete = true;

} //END function 'command_switch'
