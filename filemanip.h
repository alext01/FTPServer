/*****************************************************
 *
 * filemanip.h
 * Group Project
 * Class: CMPT 361
 * Version: 1.2ftp
 * 
 * Includes function such as reading files, finding
 * the size of the file, writing to a file, listing
 * directories, changing directories.
 *
 ****************************************************/
#define _XOPEN_SOUCE 600

#ifndef __FILEMANIP_H__
#define __FILEMANIP_H__

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#include "common.h"
#include "session.h" //Required for session_info_t in function prototype.

/*********************************************************
 *
 * openFile
 * Arguments: char * - Name of the file being read.
 *            char * - Path of the current directory
 *            char * - Characters to determine if
 *                     file is opened for read or
 *                     write. r for read b for bytes,
 *                     w for write.
 *
 * Return: pointer to the File, if NULL then error was
 *         encountered.
 * Description: To open up file of name passed to
 *    the function.
 *
 *********************************************************/
FILE * openFile(char * fileName, char * path, char * purp);

/**********************************************************
 *
 * fSzCount
 * Arguments: FILE * - Pointer to the file to be
 *                     checked
 * Return: long int - Size of the file.
 * Description: Finds the size of the file and
 *              returns it back the size of file.
 *
 *********************************************************/
long int fSzCount(FILE * fp);

/**********************************************************
 *
 * readFile
 * Arguments: FILE * - File pointer to read from
 *            long int - size of file
 * Return: void - variable not necessary as it is
 *                storing in fileBuff that is a
 *                global variable.
 * Description: Returns the data that was read from
 *              file that is opened.
 *
 **********************************************************/
void readFile(FILE * fp, long int fSize);

/***********************************************************
 *
 * writeFile
 * Arguments: FILE * - File pointer to write to
 *            long int - size of data to write
 * Return: void
 * Description: Takes data and writes it into a
 *              file.
 *
 ***********************************************************/
void writeFile(FILE * fp, long int datSZ);

/***********************************************************
 *
 * closeFile
 * Arguments: FILE * - File to close
 * Return: int - 0 on success, -1 if error encounterd
 * Descriptioon: Closes the file after we are
 *               done reading or writing to them.
 *
 **********************************************************/
int closeFile(FILE * fp);

/**********************************************************
 *
 * cmd_list_nlist
 * Arguments: const char * -
 *            const char * - argument path
 *            bool - determines if to print details or not
 *                  true for yes, false for no.
 * Return: void
 * Description: 
 * Original Author: Alex Tai
 * Checked by: Evan Myers
 *********************************************************/
void cmd_list_nlst (session_info_t *si, char *argpath, bool detail);

/**********************************************************
 *
 * listDirect
 * Argument: char * - current location
 *           int - determines if to print details or not
 *                  1 for yes, 0 for no.
 * Return: void
 * Description: List out all files and folders in
 *              the current directory.
 * Original Author: Alex Tai
 * Checked by: Evan Myers
 *********************************************************/
void listDirect(session_info_t *si, char *fullpath, bool detail);

/*********************************************************
 *
 * detailList
 * Argument: char * - filename
 * Return: int - 0 for success, -1 for error
 * Description: Display detailed information about files
 * Original Autnor: Alex Tai
 * Checked by: Evan Myers
 *********************************************************/
int detailList(struct dirent * dirInfo, char * filepath, char ** output);

/*********************************************************
 * makeDir
 * Argument: char * - filepath
 * Return: int - 0 if ran without errors, -1 if error
 *               was encountered. 
 * Description: Creates a directory
 * Original Author: Alex Tai
 * Checked by: Evan Myers
 ********************************************************/
int makeDir(session_info_t *si, char * filepath);

/******************************************************************************
 * Change the current working directory to the parent directory by calling
 * cmd_cwd() with the argument path "..".
 *
 * Argument:
 *   si  - The command thread session information.
 *   arg - The command is rejected if this argument is not NULL.
 * 
 * Origin author: Evan Myers
 * Checked by: Alex Tai
 *****************************************************************************/
void cmd_cdup (session_info_t *si, char *arg);

/******************************************************************************
 * Change the current working directory.
 *
 * Argument:
 *   si  - The command thread session information.
 *   arg - Change the current working directory to this path if accepted.
 * 
 * Origin author: Alex tai
 * Changed by: Evan Myers - using code written by James Yoo and Justin Slind
 ******************************************************************************/
void cmd_cwd (session_info_t *si, char *arg);

#endif
