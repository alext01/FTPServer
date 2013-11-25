//

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_switch.h"
#include "cmd_line_parser.h"

#define MIN_ARGS 1

void command_switch(const char *cmdLine)
{ //BEGIN function 'command_switch'

  int numArgs;
  char *cmd;

  numArgs = command_arg_count(cmdLine);

  if (numArgs >= MIN_ARGS) {

    cmd = command_extract_cmd(cmdLine);

    if (strcmp(cmd, "USER") == 0) {

      //Test print
      printf("Correct!\t%lu\t%d\n%s\n", strlen(cmd), numArgs, cmd);

    } else if (strcmp(cmd, "PASS") == 0) {



    } else if (strcmp(cmd, "QUIT") == 0) {



    } else if (strcmp(cmd, "PORT") == 0) {



    } else if (strcmp(cmd, "PASV") == 0) {



    } else if (strcmp(cmd, "TYPE") == 0) {



    } else if (strcmp(cmd, "STRU") == 0) {



    } else if (strcmp(cmd, "MODE") == 0) {



    } else if (strcmp(cmd, "RETR") == 0) {



    } else if (strcmp(cmd, "STOR") == 0) {



    } else if (strcmp(cmd, "NOOP") == 0) {



    } else {

      printf("Nope.\t%lu\n", strlen(cmd));

    } //END statement 'if-else'
    
    free(cmd);

  } else {

    printf("ERROR: Insufficient Arguments!\n");

  }

} //END function 'command_switch'
