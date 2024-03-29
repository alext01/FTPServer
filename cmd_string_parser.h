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
//    cmd_string_parser.h
//===============================================================================
//  Associated Header File(s):
//    > N/A
//===============================================================================
//  Brief Description:
//    The following functions perform parsing operations on an input string that
//    contains an FTP command along with all of its relevant arguments.
//===============================================================================
//  Function Listing (in order of entry):
//    > command_arg_count
//    > command_extract_arg
//    > command_extract_cmd
//    > strdup
//    > convert_to_upper
//    > trim_whitespace
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//    > http://www.stackoverflow.com/
//===============================================================================




//BEGIN macro guard '__CMD_STRING_PARSER_H__'
#ifndef __CMD_STRING_PARSER_H__
#define __CMD_STRING_PARSER_H__




//===============================================================================
//  Function Name:
//    command_arg_count(const char *cmdString)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {cmdString}
//      > Type constant character pointer
//      > Contains the entirety of the command string entered by the client
//===============================================================================
//  Return Value(s):
//    
//===============================================================================

int command_arg_count(const char *cmdString);




//===============================================================================
//  Function Name:
//    command_extract_arg(const char *cmdString)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {cmdString}
//      > Type constant character pointer
//      > Contains the entirety of the command string entered by the client
//===============================================================================
//  Return Value(s):
//    
//===============================================================================

char *command_extract_arg(const char *cmdString);




//===============================================================================
//  Function Name:
//    command_extract_cmd(const char *cmdString)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {cmdString}
//      > Type constant character pointer
//      > Contains the entirety of the command string entered by the client
//===============================================================================
//  Return Value(s):
//    
//===============================================================================

char *command_extract_cmd(const char *cmdString);




//===============================================================================
//  Function Name:
//    strdup(const char *string)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {string}
//      > Type constant character pointer
//      > 
//===============================================================================
//  Return Value(s):
//    
//===============================================================================

char *strdup(const char *string);




//===============================================================================
//  Function Name:
//    convert_to_upper(char *string)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {string}
//      > Type character pointer
//      > 
//===============================================================================
//  Return Value(s):
//    
//===============================================================================

void convert_to_upper(char *string);




//===============================================================================
//  Function Name:
//    trim_whitespace(char *string)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {string}
//      > Type character pointer
//      > 
//===============================================================================
//  Return Value(s):
//    
//===============================================================================

void trim_whitespace(char *string);




//END macro guard '__CMD_STRING_PARSER_H__'
#endif
