/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: filetemp.c
 * Date: November 2013
 *
 * NOTE:
 *   I am unsure which file these functions should appear in at this time. Will
 * find an appropriate name or file in the near futer.
 *****************************************************************************/
#include <string.h>
#include "filetemp.h"

/******************************************************************************
 * check_legal_path
 *****************************************************************************/
bool check_legal_path (const char *cwd, const char *argpath,)
{
  bool absolute;
  char *strstr;
  char temp_str[strlen (argpath)];

  if ((strstr = strstr (argpath, "/")) == argpath)
    absolute = true;
    
}
