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
//     cmd_line_parser.c
//=============================================================================
// Brief Description:
//     The following functions perform parsing operations on an input string
//     that conatins an FTP command along with all of its relevants arguments.
//=============================================================================


//=============================================================================
// Code Citation(s):
//
//
//=============================================================================


//C Library References
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


//Header File References
#include "cmd_line_parser.h"



//=============================================================================
// Brief Description:
//     
//=============================================================================
int command_arg_count(const char *cmdString)
{ //BEGIN function 'command_arg_count'

  int argCount;

  argCount = 0;
  for (int i = 0; i < strlen(cmdString); i++) {
    if (!isspace(cmdString[i])) {
      while (!isspace(cmdString[i])) {
	i++;
      } //END loop 'while'
      argCount++;
    } //END statement 'if'
  } //END loop 'for'

  return argCount;

} //END function 'command_arg_count'



//=============================================================================
// Brief Description:
//     
//=============================================================================
char *command_extract_arg(const char *cmdString)
{ //BEGIN function 'command_extract_arg'

  char *argString, 
       *command,
       *tempString;

  tempString = strdup(cmdString);
  if ((argString = (char *)calloc((strlen(tempString) + 1), sizeof(char))) == NULL) {
    return NULL;
  } //END statement 'if'
  command = command_extract_cmd(cmdString);

  memcpy(argString, (tempString + strlen(command)), ((strlen(tempString) - strlen(command)) * sizeof(char)));
  if (strlen(argString) == 0) {
    return NULL;
  } //END statement 'if'
  trim_whitespace(argString);
  argString = (char *)realloc(argString, ((strlen(argString) + 1) * sizeof(char)));

  free(command);
  free(tempString);
  return argString;

} //END function 'command_extract_arg'



//=============================================================================
// Brief Description:
//     
//=============================================================================
char *command_extract_cmd(const char *cmdString)
{ //BEGIN function 'command_extract'

  char *command,
       *tempString,
       *token;

  tempString = strdup(cmdString);
  if ((command = (char *)calloc(strlen(cmdString), sizeof(char))) == NULL) {
    return NULL;
  } //END statement 'if'
  token = strtok(tempString, " ");
  if ((command = (char *)realloc(command, strlen(token + 1))) == NULL) {
    return NULL;
  } //END statement 'if'
  strcpy(command, token);
  convert_to_upper(command);

  free(tempString);
  return command;

} //END function 'command_extract'



//=============================================================================
// Brief Description:
//     
//=============================================================================
char *strdup(const char *string)
{ //BEGIN function 'strdup'

  char *duplicate;

  if ((duplicate = (char *)calloc((strlen(string) + 1), sizeof(char))) == NULL) {
    return NULL;
  } //END statement 'if'
  strcpy(duplicate, string);
  trim_whitespace(duplicate);

  return duplicate;

} //END function 'strdup'



//=============================================================================
// Brief Description:
//     
//=============================================================================
void convert_to_upper(char *string)
{ //BEGIN function 'conver_to_upper'

  for (int i = 0; i < strlen(string); i++) {
    string[i] = toupper(string[i]);
  } //END loop 'for'

} //END function 'convert_to_upper'



//=============================================================================
// Brief Description:
//     
//=============================================================================
void trim_whitespace(char *string)
{ //BEGIN function 'trim_whitespace'

  int length;
  char *lead,
       *tail;

  length = strlen(string);
  lead = string - 1;
  tail = string + length;

  while (isspace(*(++lead)));
  while (isspace(*(--tail)) && (tail != lead));

  if ((string + length - 1) != tail) {
    *(tail + 1) = '\0';
  } else if ((lead != string) && (tail == lead)) {
    *string = '\0';
  } //END statement 'if-else'

  tail = string;
  if (lead != string) {
    while (*lead) *tail++ = *lead++;
    *tail = '\0';
  } //END statement 'if'

} //END function 'trim_whitespace'
