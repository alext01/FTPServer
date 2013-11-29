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
 *
 * Acknowledgements:
 *   We wanted to create a function that determines if a pathname argument for
 *   a command received from a client was appropriate.
 *
 *   One of the deciding factors to determine if the pathname was appropriate
 *   was to ensure that the pathname argument is a descendant of our servers
 *   chosen root directory.
 *
 *   When researching realpath() and PATH_MAX I determined that at this point
 *   in time realpath() is not safe or not portable.
 *
 *   I created a function to canonicalize a pathname, but struggled to resolve
 *   symbolic links.
 *
 *   While researching a solution to resolve symbolic links, I found this
 *   webpage:
 *   http://www.gnu.org/software/libc/manual/html_node/Symbolic-Links.html
 *
 *   In this webpage, the function canonicalize_file_name() appears, which I
 *   have used in my final solution. It was the solution to the original
 *   problem.
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


extern char *rootdir;  //defined in the file 'main.c'


char *merge_paths (const char *cwd, const char *argpath,
			  const int *reserve);
static bool within_rootdir (char *fullpath, const char *trimmed);
static bool is_a_dir (const char *fullpath);
static int is_unique (const char *fullpath);
static char *trim_arg_path (char **argpath, int *reserve);
static void restore_trimmed (char **argpath, char **fullpath, char *trimmed);


/******************************************************************************
 * check_file_exist
 *****************************************************************************/
bool check_file_exist (const char *cwd, const char *argpath)
{
  char *fullpath;

  //Merge all parts of the pathname to create a single pathname.
  if ((fullpath = merge_paths (cwd, argpath, NULL)) == NULL)
    return false;

  //Ensure the pathname is a descendant of the servers root directory.
  if (!within_rootdir (fullpath, NULL)) {
    free (fullpath);
    return false;
  }

  free (fullpath);
  return true;
}


/******************************************************************************
 * check_dir_exist
 *****************************************************************************/
bool check_dir_exist (const char *cwd, const char *argpath)
{
  char *fullpath;

  //Merge all parts of the pathname to create a single pathname.
  if ((fullpath = merge_paths (cwd, argpath, NULL)) == NULL)
    return false;

  //Determine if the pathname is a descendant of the servers root directory.
  if (!within_rootdir (fullpath, NULL)) {
    free (fullpath);
    return false;
  }

  //Determine if the pathname is for a directory.
  if (!is_a_dir (fullpath)) {
    free (fullpath);
    return false;
  }

  free (fullpath);
  return true;
}


/******************************************************************************
 * check_futer_file
 *****************************************************************************/
int check_futer_file (const char *cwd, char *argpath)
{
  char *fullpath;
  char *trimmed;
  int reserve;

  if ((trimmed = trim_arg_path (&argpath, &reserve)) == NULL)
    return -1;

  if ((fullpath = merge_paths (cwd, argpath, &reserve)) == NULL)
    return -1;

  if (!within_rootdir (fullpath, trimmed)) {
    free (fullpath);
    free (trimmed);
    return -2;
  }

  //Trimmed is freed in this function.
  restore_trimmed (&argpath, &fullpath, trimmed);

  /* If the pathname argument received from the client is the pathname of a
   * directory, the new file cannot be created. */
  if (is_a_dir (fullpath)) {
    free (fullpath);
    return -3;
  }

  free (fullpath);
  return 0;
}

/******************************************************************************
 * merge_paths - see path.h
 *****************************************************************************/
char *merge_paths (const char *cwd, const char *argpath, const int *reserve)
{
  char *fullpath;    //Concatenate the rootdir, cwd, and path arg.

  //String lengths required to malloc the fullpath string.
  int rootdir_strlen;
  int cwd_strlen;
  int arg_strlen;

  rootdir_strlen = strlen (rootdir) + 1;
  cwd_strlen = strlen (cwd) + 1;
  arg_strlen = strlen (argpath) + 1;

  if (reserve != NULL)
    arg_strlen += *reserve;

  if ((fullpath = malloc ((rootdir_strlen + cwd_strlen + arg_strlen)
			  * sizeof (*fullpath))) == NULL) {
    fprintf (stderr, "%s: malloc: could not allocate required space\n",
	     __FUNCTION__);
    return NULL;
  }

  //Create the complete pathname argument. 
  strcpy (fullpath, rootdir);
  strcat (fullpath, cwd);
  strcat (fullpath, argpath);

  return fullpath;
}


/******************************************************************************
 * within_rootdir
 *****************************************************************************/
static bool within_rootdir (char *fullpath, const char *trimmed)
{
  char *canon;       //An abreviation of canonicalized absolute pathname.
  char *str;         //Return value of strcat function.

  //Resolve all "..", ".", and duplicate '/' entries. Resolve symbolic links.
  if ((canon = canonicalize_file_name (fullpath)) == NULL) {
    fprintf (stderr, "%s: canonicalize_file_name: %s\n", __FUNCTION__, 
	     strerror (errno));
    return false;
  }

  //Determine if the pathname is a descendant of the servers root directory.
  if (strstr (canon, rootdir) == NULL) {
    /* This if statement handles a very special case. See the section 'notes'
     * in the function header. */
    if (trimmed != NULL) {
      str = strcat (fullpath, trimmed);
      if (!within_rootdir (fullpath, NULL)) {
	*str = '\0';
	free (canon);
	return false;
      }
      *str = '\0';
    } else {
      fprintf (stderr, "%s: pathname out of rootdir scope\n", __FUNCTION__);
      free (canon);
      return false;
    }
  }

  free (canon);
  return true;
}


/******************************************************************************
 * is_a_dir
 *****************************************************************************/
bool is_a_dir (const char *fullpath)
{
  struct stat st;  //Used to check if the file at pathname is a directory.

  //stat() the file to determine if the file is a directory.
  if (stat (fullpath, &st) == -1) {
    fprintf (stderr, "%s: stat: %s\n", __FUNCTION__, strerror (errno));
    return false;
  }

  //Determine if the file is a directory. See man (2) stat, line 120.
  if (!(st.st_mode & S_IFDIR)) {
    fprintf (stderr, "%s: file is not a directory when directory expected\n",
	     __FUNCTION__);
    return false;
  }
  return true;
}


/******************************************************************************
 * trim_arg_path
 *****************************************************************************/
static char *trim_arg_path (char **argpath, int *reserve)
{
  char *trim_filen = NULL;  //Store the trimmed portion of the filename.
  char *str;                //Used to collect the filename from the prefix path.

  //Determine if the argument contains a pathname prefix.
  if ((str = strrchr(*argpath, '/')) != NULL) {
    if (strlen (*argpath) > 1) //required when the pathname argument is "/".
      str++;              //Do not include the '/' prefix in the new filename.
  } else {
    //The filename does not include a path prefix (eg. no '/' is present).
    str = *argpath;
  }

  /* Do not remove any trailing ".." entries. This function is always called
   * before resolving the pathname. */
  if (strcmp (str, "..") == 0)
    str = NULL;

  //Nothing has been trimmed. Set return values and exit the function.
  if (str == NULL) {
    *reserve = 0;
    return NULL;
  }
  
  //Set the amount to be trimmed.
  *reserve = strlen (*argpath) + 1;

  /* Collect the filename from the prefix path.
   * eg. collect "filename" from "prefix/filename". */
  if ((trim_filen = malloc (*reserve * sizeof (*trim_filen))) == NULL) {
    fprintf (stderr, "%s: malloc: could not allocate required space\n",
	     __FUNCTION__);
    return NULL;
  }

  /* Copy the string and emove the filename from the prefix path. This
   * operation will replace a trailing filename with a null character so that
   * futer string functions only recognize the prefix.
   * eg. "prefix/trail" becomes "prefix/" */
  strncpy (trim_filen, str, *reserve);
  *str = '\0';
  
  return trim_filen;
}


/******************************************************************************
 * restore_trimmed
 *****************************************************************************/
static void restore_trimmed (char **argpath, char **fullpath, char *trimmed)
{
  strcat (*argpath, trimmed);
  strcat (*fullpath, trimmed);
  free (trimmed);
}


/******************************************************************************
 * is_unique
 *****************************************************************************/
static int is_unique (const char *fullpath)
{
  struct stat st;  //Used to check if the file at pathname is a directory.

  if (stat (fullpath, &st) == -1) {
    if (errno == ENOENT) { //Return true if the file does not exist.
      return 0;
    } else {
      fprintf (stderr, "%s: stat: %s\n", __FUNCTION__, strerror (errno));
      return -1;
    }
  } else {
    fprintf (stderr, "%s: stat: FUTER_UNIQ file exists\n", __FUNCTION__);
    return 1;
  }
}
