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


static char *merge_paths (const char *cwd, const char *argpath, int *reserve);
static bool within_rootdir (const char *fullpath);
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
  int reserve;

  //Merge all parts of the pathname to create a single pathname.
  if ((fullpath = merge_paths (cwd, argpath, &reserve)) == NULL)
    return false;

  //Ensure the pathname is a descendant of the servers root directory.
  if (!within_rootdir (fullpath)) {
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
  int reserve;

  //Merge all parts of the pathname to create a single pathname.
  if ((fullpath = merge_paths (cwd, argpath, &reserve)) == NULL)
    return false;

  //Determine if the pathname is a descendant of the servers root directory.
  if (!within_rootdir (fullpath)) {
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


  return 0;

}

/******************************************************************************
 * merge_paths - see path.h
 *****************************************************************************/
static char *merge_paths (const char *cwd, const char *argpath, int *reserve)
{
  char *fullpath;    //Concatenate the rootdir, cwd, and path arg.

  //String lengths required to malloc the fullpath string.
  int rootdir_strlen;
  int cwd_strlen;
  int arg_strlen;

  rootdir_strlen = strlen (rootdir) + 1;
  cwd_strlen = strlen (cwd) + 1;
  arg_strlen = strlen (argpath) + 1;

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
static bool within_rootdir (const char *fullpath)
{
  char *canon;       //An abreviation of canonicalized absolute pathname.

  //Resolve all "..", ".", and duplicate '/' entries. Resolve symbolic links.
  if ((canon = canonicalize_file_name (fullpath)) == NULL) {
    fprintf (stderr, "%s: canonicalize_file_name: %s\n", __FUNCTION__, 
	     strerror (errno));
    return false;
  }

  //Determine if the pathname is a descendant of the servers root directory.
  if (strstr (canon, rootdir) == NULL) {
    fprintf (stderr, "%s: pathname out of rootdir scope\n", __FUNCTION__);
    free (canon);
    return false;
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
  int trim_strlen = 0;      //Used to malloc new_filen.
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
    *trimmed = 0;
    return NULL;
  }
  
  //Set the amount to be trimmed.
  trim_strlen = strlen (*argpath) + 1;
  *trimmed = trim_strlen;

  /* Collect the filename from the prefix path.
   * eg. collect "filename" from "prefix/filename". */
  if ((trim_filen = malloc (trim_strlen * sizeof (*trim_filen))) == NULL) {
    fprintf (stderr, "%s: malloc: could not allocate required space\n",
	     __FUNCTION__);
    return NULL;
  }

  /* Copy the string and emove the filename from the prefix path. This
   * operation will replace a trailing filename with a null character so that
   * futer string functions only recognize the prefix.
   * eg. "prefix/trail" becomes "prefix/" */
  strncpy (trim_filen, str, trim_strlen);
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


/******************************************************************************
 * accept_path
 *
 * -Function is being replaced by smaller functions.
 * -Consider this depreciated.
 *****************************************************************************/
bool accept_path (session_info_t session, char *arg, int mode)
{
  char *fullpath;    //Concatenate the rootdir, cwd, and path arg.
  char *canon;       //An abreviation of canonicalized absolute pathname.
 
  //Required for mode: FUTER_UNIQ
  char *uniq_filen = NULL; //The part of the pathname that should not exist.
  int uniq_strlen;         //Used to malloc new_filen.

  //Required for modes: FUTER_UNIQ, FUTER_FILE
  char *str;  //Used to collect (or remove) the filename from the prefix path.

  //String lengths required to malloc the fullpath string.
  int rootdir_strlen;
  int cwd_strlen;
  int arg_strlen;

  struct stat st;  //Used to check if the file at pathname is a directory.

  /* When only the prefix of the pathname must be checked for correctness,
   * remove the filename from the file path. If no prefix component is present
   * remove the argument completely. The function canonicalize_file_name()
   * appearing later in this function will return error if a filename is
   * present in the path argument that is not present on the filesystem. 
   *
   * When determining if a file can be created, what is important is that
   * every file found in the prefix is a directory. */
  if ((mode == FUTER_FILE) || (mode == FUTER_UNIQ)) {
    if ((str = strrchr(arg, '/')) != NULL) {
      if (strlen (arg) > 1)//required for the pathname argument "/".
	str++;             //Do not include the '/' prefix in the new filename.
    } else {
      str = arg;
    }

    if (strcmp (str, "..") == 0)
      str = NULL;
    else if (strcmp (str, "../") == 0)
      str = NULL;

    if (((mode == FUTER_UNIQ) || (mode == FUTER_FILE)) && (str != NULL)) {
      uniq_strlen = strlen (arg) + 1;
      if ((uniq_filen = malloc (uniq_strlen * sizeof (*uniq_filen))) == NULL) {
	fprintf (stderr, "%s: malloc: could not allocate required space\n",
		 __FUNCTION__);
	return false;
      }

      strncpy (uniq_filen, str, uniq_strlen);
    }

    if (str != NULL)
      *str = '\0'; //Remove the futer file from the file path.
  }


  //Calculate the malloc size of the fullpath string.
  rootdir_strlen = strlen (rootdir) + 1;
  cwd_strlen = strlen (session.cwd) + 1;
  arg_strlen = strlen (arg) + 1;

  /* Malloc space for the unique filename, so it can be reattached later. When
   * it is determined whether a file already exists with that name. */
  if (mode == FUTER_UNIQ)
    arg_strlen += uniq_strlen;


  if ((fullpath = malloc ((rootdir_strlen + cwd_strlen + arg_strlen)
			  * sizeof (*fullpath))) == NULL) {
    fprintf (stderr, "%s: malloc: could not allocate required space\n",
	     __FUNCTION__);
    if (uniq_filen != NULL)
      free (uniq_filen);
    return false;
  }

  //Create the complete pathname argument. 
  strcpy (fullpath, rootdir);
  strcat (fullpath, session.cwd);
  strcat (fullpath, arg);

  printf ("rootdir = %s\n", rootdir);
  printf ("fullpath = %s\n", fullpath);

  //Resolve all "..", ".", and duplicate '/' entries. Resolve symbolic links.
  if ((canon = canonicalize_file_name (fullpath)) == NULL) {
    fprintf (stderr, "%s: canonicalize_file_name: %s\n", __FUNCTION__, 
	     strerror (errno));
    free (fullpath);
    if (uniq_filen != NULL)
      free (uniq_filen);
    return false;
  }
  free (fullpath);

  printf ("canon = %s\n", canon);

  //Determine if the pathname is a descendant of the servers root directory.
  if (strstr (canon, rootdir) == NULL) {
    fprintf (stderr, "%s: pathname out of rootdir scope\n", __FUNCTION__);
    if (uniq_filen != NULL)
      free (uniq_filen);
    free (canon);
    return false;
  }

  //Nothing else is required when checking if the file exists.
  if (mode == CUR_FILE) {
    free (canon);
    return true;
  }


  /* In these modes, the pathname stored in the string canon is expected to be
   * a directory. */
  if ((mode == FUTER_FILE) || (mode == CUR_DIR)) {
    if (stat (canon, &st) == -1) {
      fprintf (stderr, "%s: stat: %s\n", __FUNCTION__, strerror (errno));
      free (canon);
      if (uniq_filen != NULL)
	free (uniq_filen);
      return false;
    } else {
      if (FUTER_FILE && (uniq_filen != NULL)) {
	strcat (arg, uniq_filen);
	free (uniq_filen);
	return true;
      }
      free (canon);
    }

    //Determine if the file is a directory. See man (2) stat, line 120.
    if (!(st.st_mode & S_IFDIR)) {
      fprintf (stderr, "%s: file is not a directory when directory expected\n",
	       __FUNCTION__);
      return false;
    }
    return true;
  }

  if (mode == FUTER_UNIQ) {
    //Reattach the unique filename to determine if the file exists.
    strcat (canon, uniq_filen);
    strcat (arg, uniq_filen);
    free (uniq_filen);
    if (stat (canon, &st) == -1) {
      if (errno == ENOENT) { //Return true if the file does not exist.
	free (canon);
	return true;
      } else {
	fprintf (stderr, "%s: stat: %s\n", __FUNCTION__, strerror (errno));
	free (canon);
	return false;
      }
    } else {
      fprintf (stderr, "%s: stat: FUTER_UNIQ file exists\n", __FUNCTION__);
      free (canon);
      return false;
    }
  }
    
  free (canon);
  return true;
}
