/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: cmd_misc.h
 * Date: November 2013
 *
 * Description:
 *   Miscellaneous ftp commands that don't fit in other categories
 *****************************************************************************/

#ifndef __CMD_MISC_H__
#define __CMD_MISC_H__
#include "session.h"
#define MAX_NUM_ARGS 1


/******************************************************************************
 * Changes Type.  Valid types are ASCII Non-print and Binary
 *
 * Arguments:
 *   si - info for current session
 *   arg - current command with parameter
 *
 * Returns
 * 	void
 *
 * Original author: Justin Slind
 *****************************************************************************/

void cmd_type(session_info_t *si, char *arg);


/******************************************************************************
 * Changes Mode.  Only valid mode is stream
 *
 * Arguments:
 *   arg - current command with parameter
 *
 * Returns
 * 	void
 *
 * Original author: Justin Slind
 *****************************************************************************/
void cmd_mode(session_info_t *si, char *arg);

/******************************************************************************
 * Display the system type.
 *
 * Arguments:
 *   si - The control thread session information.
 *
 * Original author: Evan Myers
 *
 * Acknowledgements:
 *   http://cr.yp.to/ftp/syst.html
 *
 *   The author states "I recommend that all new servers respond to SYST with
 *   the meaningless string '215 UNIX Type: L8' exactly as used by the majority
 *   of current servers."
 *
 *   I have decided to follow this advice.
 *
 *   Also, ftp.gnu.org sends this message as a reply to the SYST command, and
 *   our server was created for a UNIX system.
 *****************************************************************************/
void cmd_syst (session_info_t *si);


//===============================================================================
//  Function Name:
//    cmd_stru(session_info_t *si, char *arg, int argCount)
//===============================================================================
//  Description:
//    
//===============================================================================
//  Parameter Listing (in order of entry):
//    {si}
//      > Type structure pointer
//      > Contains all pertinent information in regards to the current client
//    {arg}
//      > Type character pointer
//      > Contains the entire argument string (excludes the command)
//    {argCount}
//      > Type integer
//      > Contains the number of arguments passed from the command string (with)
//        the command itself being excluded from the count)
//===============================================================================
//  Return Value(s):
//    
//===============================================================================
void cmd_stru(session_info_t *si, char *arg, int argCount);

#endif //__CMD_MISC_H__
