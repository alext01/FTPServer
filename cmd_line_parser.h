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
//     cmd_line_parser.h
//=============================================================================
// Brief Description:
//     The following function prototypes perform parsing operations on an
//     input string that contains an FTP command along with all of its
//     relevant arguments.
//=============================================================================


//BEGIN macro guard '_CMD_LINE_PARSER_H_'
#ifndef _CMD_LINE_PARSER_H_
#define _CMD_LINE_PARSER_H_


int command_arg_count(const char *cmdString);

char *command_extract_arg(const char *cmdString);
char *command_extract_cmd(const char *cmdString);
char *strdup(const char *string);

void convert_to_upper(char *string);
void trim_whitespace(char *string);


//END macro guard '_CMD_LINE_PARSER_H_'
#endif
