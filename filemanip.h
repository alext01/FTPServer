/*****************************************************
 *
 * filemanip.h
 * Group Project
 * Class: CMPT 361
 * Version: 1.2ftp
 * Author: Alex Tai
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
 * listDirect
 * Argument: char * - current location
 * Return: char * - return a string of directories
 * Description: List out all files and folders in
 *              the current directory.
 *
 *********************************************************/
char * listDirect(char * curloc);

/*********************************************************
 *
 * changeDirect
 * Argument: char * - current location
 *           char * - changes in location
 * Return: char * - new overall pathname
 * Description: Change the current working directory
 *
 ********************************************************/
char * changeDirect(char * curloc, char * directChanges);

#endif
