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
//    cmd_string_parser.c
//===============================================================================
//  Associated Header File(s):
//    > cmd_string_parser.h
//===============================================================================
//  Overview:
//    The following functions perform parsing operations on an input string that
//    contains an FTP command along with all of its relevant arguments.
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.stackoverflow.com/
//===============================================================================




//C Library Reference(s)
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


//Header File Reference(s)
#include "cmd_string_parser.h"


//Preprocessor Macro Define(s)
#define MIN_NUM_ARGS 2    //Minimum allowed number of arguments for command_extract_arg()




//===============================================================================
//  Function Name:
//    command_arg_count(const char *cmdString)
//===============================================================================
//  Description:
//    Consult file "cmd_string_parser.h"
//===============================================================================
//  Variable List (in alphabetic order):
//    {argCount}
//      > Type integer
//      > Contains the count of the number of arguments within the given command
//        string
//    {i}
//      > Type integer
//      > Contains the 'for' loop counter and array indexer
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

int command_arg_count(const char *cmdString)
{ //BEGIN function 'command_arg_count'

  int argCount;

  argCount = 0;
  for (int i = 0; i < strlen(cmdString); i++) {
    if (!isspace(cmdString[i])) {
      while (!isspace(cmdString[i]) && (i < strlen(cmdString))) {
	i++;
      } //END loop 'while'
      argCount++;
    } //END statement 'if'
  } //END loop 'for'

  return argCount;

} //END function 'command_arg_count'




//===============================================================================
//  Function Name:
//    command_extract_arg(const char *cmdString)
//===============================================================================
//  Description:
//    Consult file "cmd_string_parser.h"
//===============================================================================
//  Variable List (in alphabetic order):
//    {argCount}
//      > Type integer
//      > Contains the count of the number of arguments within the given command
//        string
//    {argString}
//      > Type character pointer
//      > Contains the entire argument string (excludes the command)
//    {command}
//      > Type character pointer
//      > Contains the invoked command (excludes the argument string)
//    {tempString}
//      > Type character pointer
//      > Contains a duplicate of the original command string
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

char *command_extract_arg(const char *cmdString)
{ //BEGIN function 'command_extract_arg'

  int argCount;

  char *argString, 
       *command,
       *tempString;

  argCount = command_arg_count(cmdString);

  if (argCount < MIN_NUM_ARGS) {
    return NULL;
  } //END statement 'if'

  tempString = strdup(cmdString);

  if ((argString = (char *)calloc((strlen(tempString) + 1), sizeof(char))) == NULL) {
    free(tempString);
    return NULL;
  } //END statement 'if'

  command = command_extract_cmd(cmdString);
  memcpy(argString, (tempString + strlen(command)), ((strlen(tempString) - strlen(command)) * sizeof(char)));
  trim_whitespace(argString);

  if ((argString = (char *)realloc(argString, ((strlen(argString) + 1) * sizeof(char)))) == NULL) {
    free (command);
    free (tempString);
    return NULL;
  } //END statement 'if'

  free(command);
  free(tempString);

  return argString;

} //END function 'command_extract_arg'




//===============================================================================
//  Function Name:
//    command_extract_cmd(const char *cmdString)
//===============================================================================
//  Description:
//    Consult file "cmd_string_parser.h"
//===============================================================================
//  Variable List (in alphabetic order):
//    {command}
//      > Type character pointer
//      > Contains the invoked command (excludes the argument string)
//    {tempString}
//      > Type character pointer
//      > Contains a duplicate of the original command string
//    {token}
//      > Type character pointer
//      > Contains a string token from 'tempString' delimited by spaces
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

char *command_extract_cmd(const char *cmdString)
{ //BEGIN function 'command_extract'

  char *command,
       *tempString,
       *token;

  tempString = strdup(cmdString);

  if ((command = (char *)calloc(strlen(tempString), sizeof(char))) == NULL) {
    free(tempString);
    return NULL;
  } //END statement 'if'

  token = strtok(tempString, " ");

  if ((command = (char *)realloc(command, ((strlen(token) + 1) * sizeof(char)))) == NULL) {
    free(tempString);
    return NULL;
  } //END statement 'if'

  strcpy(command, token);
  convert_to_upper(command);

  free(tempString);

  return command;

} //END function 'command_extract'




//===============================================================================
//  Function Name:
//    strdup(const char *string)
//===============================================================================
//  Description:
//    Consult file "cmd_string_parser.h"
//===============================================================================
//  Variable List (in alphabetic order):
//    {duplicate}
//      > Type character pointer
//      > Contains a duplicate of the original command string
//===============================================================================
//  Related Citation(s):
//    > http://www.stackoverflow.com/questions/252782/strdup-what-does-it-do-in-c/
//===============================================================================

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




//===============================================================================
//  Function Name:
//    convert_to_upper(char *string)
//===============================================================================
//  Description:
//    Consult file "cmd_string_parser.h"
//===============================================================================
//  Variable List (in alphabetic order):
//    {i}
//      > Type integer
//      > Contains the 'for' loop counter and array indexer
//===============================================================================
//  Related Citation(s):
//    > N/A
//===============================================================================

void convert_to_upper(char *string)
{ //BEGIN function 'conver_to_upper'

  for (int i = 0; i < strlen(string); i++) {
    string[i] = toupper(string[i]);
  } //END loop 'for'

} //END function 'convert_to_upper'




//===============================================================================
//  Function Name:
//    trim_whitespace(char *string)
//===============================================================================
//  Description:
//    Consult file "cmd_string_parser.h"
//===============================================================================
//  Variable List (in alphabetic order):
//    {length}
//      > Type integer
//      > Contains the length of the given string
//    {head}
//      > Type character pointer
//      > Contains a pointer to the beginning of the character array
//    {tail}
//      > Type character pointer
//      > Contains a pointer to the end of the character array
//===============================================================================
//  Related Citation(s):
//    > http://www.stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way/
//===============================================================================

void trim_whitespace(char *string)
{ //BEGIN function 'trim_whitespace'

  int length;

  char *head,
       *tail;

  length = strlen(string);
  head = string - 1;
  tail = string + length;

  while (isspace(*(++head)));
  while (isspace(*(--tail)) && (tail != head));

  if ((string + length - 1) != tail) {
    *(tail + 1) = '\0';
  } else if ((head != string) && (tail == head)) {
    *string = '\0';
  } //END statement 'if-else'

  tail = string;

  if (head != string) {
    while (*head) *tail++ = *head++;
    *tail = '\0';
  } //END statement 'if'

} //END function 'trim_whitespace'
