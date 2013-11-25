//
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "cmd_line_parser.h"

//
char *command_extract_cmd(const char *cmdString)
{ //BEGIN function 'command_extract'

  char *command,
       *tempString,
       *token;

  tempString = strdup(cmdString);
  command = (char *)calloc(strlen(cmdString), sizeof(char));
  token = strtok(tempString, " ");
  command = (char *)realloc(command, strlen(token + 1));
  strcpy(command, token);
  convert_to_upper(command);

  free(tempString);
  return command;

} //END function 'command_extract'


//
char *command_extract_arg(const char *cmdString)
{ //BEGIN function 'command_extract_arg'

  char *argString, 
       *command;

  argString = (char *)calloc(strlen(cmdString), sizeof(char));

  command = command_extract_cmd(cmdString);
  memcpy(argString, (cmdString + strlen(command)), ((strlen(cmdString) - strlen(command)) * sizeof(char)));
  trim_whitespace(argString);
  argString = (char *)realloc(argString, strlen(argString + 1));

  free(command);
  return argString;

} //END function 'command_extract_arg'


//
void command_arg_parser()
{ //START function 'command_arg_parser'

  

} //END function 'command_arg_parser'


//
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


//
void convert_to_upper(char *string)
{ //BEGIN function 'conver_to_upper'

  for (int i = 0; i < strlen(string); i++) {
    string[i] = toupper(string[i]);
  } //END loop 'for'

} //END function 'convert_to_upper'


//
char *strdup(const char *string)
{ //BEGIN function 'strdup'

  char *duplicate;

  duplicate = (char *)calloc((strlen(string) + 1), sizeof(char));
  if (duplicate == NULL) {
    return NULL;
  } //END statement 'if'
  strcpy(duplicate, string);
  trim_whitespace(duplicate);

  return duplicate;

} //END function 'strdup'


//
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
