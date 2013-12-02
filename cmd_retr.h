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
//    cmd_retr.h
//===============================================================================
//  Associated Header File(s):
//    > session.h
//===============================================================================
//  Overview:
//    
//    
//    
//===============================================================================
//  Function Listing (in order of entry):
//    > command_retrieve
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.beej.us/guide/bgnet/output/html/singlepage/bgnet.html
//    > http://www.stackoverflow.com/
//===============================================================================




//BEGIN macro guard '__CMD_RETR_H__'
#ifndef __CMD_RETR_H__
#define __CMD_RETR_H__


//Header File Reference(s)
#include "session.h"




//===============================================================================
//  Function Name:
//    command_retrieve(session_info_t *si, char *path)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter Listing (in order of entry):
//    {si}
//      > Type structure pointer
//      > Contains all pertinent information in regards to the current client
//    {path}
//      > Type character pointer
//      > 
//===============================================================================
//  Return Value(s):
//    
//===============================================================================
void command_retrieve(session_info_t *si, char *path);




//END macro guard '__CMD_RETR_H__'
#endif
