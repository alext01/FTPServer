#ifndef __CMD_MISC_H__
#define __CMD_MISC_H__
#include "session.h"


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


#endif //__CMD_MISC_H__
