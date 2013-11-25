//=======================================================================
// Assignment #03 - FTP Server
//=======================================================================
// Date:        November 2013
// Course:      CMPT 361 - Introduction to Networks
// Instructor:  Dr. Nicholas M. Boers
// Students:    Evan Myers
//              Justin Slind
//              Alex Tai
//              James Yoo
//=======================================================================


//=======================================================================
// Filename:
//     cmd_switch.c
//=======================================================================
// Brief Description:
//
//
//
//=======================================================================


//C Library References
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


//Header File References
#include "cmd_switch.h"
#include "cmd_line_parser.h"
#include "session.h"

#define MIN_ARGS 1

void command_switch(void * arg)
{ //BEGIN function 'command_switch'

  session_info_t *si = (session_info_t*)arg;
  int numArgs;

  char *cmd;


  char *cmdLine = si->cmd_string;


  numArgs = command_arg_count(cmdLine);

  if (numArgs >= MIN_ARGS) {

    cmd = command_extract_cmd(cmdLine);
    arg = command_extract_arg(cmdLine);

    if (strcmp(cmd, "USER") == 0) {

      //Test print
      printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

    } else if (strcmp(cmd, "PASS") == 0) {

      //Test print
      printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

    } else if (strcmp(cmd, "QUIT") == 0) {

      //Test print
      printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

    } else if (strcmp(cmd, "PORT") == 0) {

      //Test print
      printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

    } else if (strcmp(cmd, "PASV") == 0) {

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

    } else if (strcmp(cmd, "NOOP") == 0) {

      //Test print
      printf("Invoked Command <%s> with Argument(s) \"%s\"\n", cmd, arg);

    } else {

      printf("ERROR: Command not yet implemented!\n");

    } //END statement 'if-else'
    
    free(cmd);
    free(arg);

  } else {

    printf("ERROR: Insufficient Arguments!\n");

  }

} //END function 'command_switch'
