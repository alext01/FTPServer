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
//    cmd_help.h
//===============================================================================
//  Associated Header File(s):
//    > session.h
//===============================================================================
//  Overview:
//    When HELP is invoked without an argument, it will return a list of all the
//    available FTP server commands that the client is able to use.
//    When HELP is invoked with an argument, it will return a short description
//    of the specified command and how it is to be used.
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.stackoverflow.com/
//===============================================================================




//BEGIN macro guard '__CMD_HELP_H__'
#ifndef __CMD_HELP_H__
#define __CMD_HELP_H__


//Header File Reference(s)
#include "session.h"




//===============================================================================
//  Function Name:
//    command_help(session_info_t *si, char *arg)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {si}
//      > Type structure pointer
//      > Contains all pertinent information in regards to the current client
//    {arg}
//      > Type character pointer
//      > Contains the entire argument string (excludes the command)
//===============================================================================
//  Return Value:
//    
//===============================================================================

int command_help(session_info_t *si, char *arg);




//END macro guard '__CMD_HELP_H__'
#endif
