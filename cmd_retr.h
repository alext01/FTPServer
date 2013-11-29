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
// Filename:
//   cmd_retr.h
//=============================================================================
// Brief Description:
//
//
//
//=============================================================================
// Code Citation(s):
//   * http://www.cplusplus.com/reference/
//   * http://www.stackoverflow.com/
//=============================================================================

#include "session.h"

//BEGIN macro guard '__CMD_RETR_H__'
#ifndef __CMD_RETR_H__
#define __CMD_RETR_H__


void command_retrieve(session_info_t *si, char *path);


//END macro guard '__CMD_RETR_H__'
#endif
