/******************************************************************************
 * Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
 * Course: CMPT-361
 * Assignment #3 - ftp server
 * File: path.c
 * Date: November 2013
 *
 * Description:
 *   The functions on this determine if a pathname argument to a client command
 *   is acceptable for the given function.
 *****************************************************************************/
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "path.h"
#include "response.h"
#include "session.h"


extern char *rootdir;  //defined in file main.c


/******************************************************************************
 * accept_path
 *****************************************************************************/
bool accept_path (const session_info_t *session, const char *arg, int mode)
{
  char *fullpath;  //Concatenate the rootdir, cwd, and path arg.
  char *canon;     //An abreviation of canonicalized absolute pathname.
  char *str;

  //String lengths required to malloc the fullpath string.
  int rootdir_strlen;
  int cwd_strlen;
  int arg_strlen;

  struct stat st;  //Used to check if the file at pathname is a directory.

  /* When only the prefix of the pathname must be checked for correctness,
   * remove the filename from the file path. */
  if (mode == IS_FUTER_FILE) {
    if ((str = strrchr(arg, '/')) != NULL) {
      str++;
      *str = '\0';
    }
  }


  //Calculate the malloc size of the fullpath string.
  rootdir_strlen = strlen (rootdir) + 1;
  cwd_strlen = strlen (session->cwd) + 1;
  arg_strlen = strlen (arg) + 1;


  if ((fullpath = malloc ((rootdir_strlen + cwd_strlen + arg_strlen)
			  * sizeof (*fullpath))) == NULL) {
    fprintf (stderr, "%s: malloc: could not allocate required space\n",
	     __FUNCTION__);
  }


  /* Create a pathname to the argument that begins at the root directory of
   * the filesystem. */
  strcpy (fullpath, rootdir);
  strcat (fullpath, session->cwd);
  strcat (fullpath, arg);


  /* Determine if all prefix entries are directories. Expand all '..' and '.'
   * entries found in the path. */
  if ((canon = realpath (fullpath, NULL)) == NULL) {
    fprintf (stderr, "%s: realpath: %s\n", __FUNCTION__, strerror (errno));

    //One or more directories in the path prefix is not a directory.
    if (errno == ENOTDIR) {
      send_mesg_501 (session->c_sfd);  //illegal argument

      //Read or search permission was denied for a component of the prefix.
    } else if (errno == EACCES) {
      send_mesg_550 (session->c_sfd);

      //The filename does not exist.
    } else if (errno == ENOENT) {
      send_mesg_550 (session->c_sfd);

    } else {
      send_mesg_451 (session->c_sfd);
    }

    free (fullpath);
    return false;
  }
  free (fullpath);


  //Ensure the argument is a descendant of the root directory of the server.
  if (strstr (canon, rootdir) == NULL) {
    fprintf (stderr,"%s: strstr: argument not in rootdir scope\n",__FUNCTION__);
    free (canon);
    return false;
  }


  /* In these modes, the pathname stored in the string canon is expected to be
   * a directory. */
  if ((mode ==  IS_FUTER_FILE) || (mode == IS_CUR_DIR)) {
    if (stat (canon, &st) == -1) {
      fprintf (stderr, "%s: stat: %s\n", __FUNCTION__, strerror (errno));
      free (canon);
      return false;
    }

    //Determine if the file is a directory. See man (2) stat, line 120.
    if (!(st.st_mode & S_IFDIR)) {
      fprintf (stderr, "%s: file is not a directory when directory expected\n",
	       __FUNCTION__);
      free (canon);
      return false;
    }
  }

  free (canon);
  return true;
}


