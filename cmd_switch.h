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
//    cmd_switch.h
//===============================================================================
//  Associated Header File(s):
//    > N/A
//===============================================================================
//  Overview:
//    A massive 'if-else if' statement that determines the command that has been
//    invoked by the client and performs an appropriately related action.
//===============================================================================
//  Code Citation(s):
//    > http://www.cplusplus.com/reference/
//    > http://www.stackoverflow.com/
//===============================================================================




//BEGIN macro guard '__CMD_SWITCH_H__'
#ifndef __CMD_SWITCH_H__
#define __CMD_SWITCH_H__




//===============================================================================
//  Function Name:
//    command_switch(void *param)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter List (in order of entry):
//    {param}
//      > Type function pointer
//      > 
//===============================================================================
//  Return Value:
//    
//===============================================================================

void *command_switch(void *param);




//END macro guard '__CMD_SWITCH_H__'
#endif
