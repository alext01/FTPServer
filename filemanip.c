/****************************************************************
 *
 * filemanip.c
 * Group Project
 * Class: CMPT 361
 * Version: 1.2ftp
 * Author: Alex Tai
 * Includes function such as reading files, finding size of the
 * file, writing to a file, listing directories, changing
 * directories.
 *
 ***************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "filemanip.h"
#include "path.h"

#define MAX_FDATSZ 4096

char fileBuff[MAX_FDATSZ];

FILE * openFile(char * fileName, char * path, char * purp){
  FILE *fp;

  if((fp = fopen(fileName, purp)) == NULL)
    {
      fprintf(stderr, "Cannot open file: %s\n", fileName);
      return NULL;
    }

  printf("file open success\n");
  return fp;
}

int closeFile(FILE *fp){
  int errchk;
  errno = 0;

  errchk = fclose(fp);
  if(errno){
    fprintf(stderr, "Error in function closeFile: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

long int fSzCount(FILE * fp){
  long int fileSZ = 0;
  int errck;
  errno = 0;

  if( (errck = fseek(fp, 0, SEEK_END)) == -1){
    fprintf(stderr, "Error: seeking EOF\n");
    return -1;
  }

  fileSZ = ftell(fp);
  rewind(fp);

  return fileSZ;
}


void readFile(FILE * fp, long int fSize){
  int errck = 0;
  // byte_t fileChunk[MAX_FDATSZ]; //need size of the data buffer

  errck = fread(fileBuff, sizeof(fileBuff[0]), MAX_FDATSZ, fp);
  if(!errck){
    if(ferror(fp)){
      fprintf(stderr, "Error in trying to read from file.\n");
      exit(1);
    }
    else if(feof(fp)){
      printf("EOF\n");
      return;
    }
      //break;
  }
  // needs more modification
}


void writeFile(FILE * fp, long int datSZ){
  int errck = 0;
  errck = fwrite(&fileBuff, sizeof(byte_t), datSZ, fp);

  if(!errck){
    if(ferror(fp)){
      fprintf(stderr, "Error from trying to write to file.\n");
      exit(1);
    } 
  }
}


int cmd_list (const char *cwd, const char *argpath)
{
  char *fullpath;

  //Determine if the file is a directory.
  if (!check_dir_exist (cwd, argpath))
    return false;

  //Create a single pathname to the directory from the pathname fragments.
  if ((fullpath = merge_paths (cwd, argpath, NULL)) == NULL)
    return false;
  
  listDirect (fullpath);
}

//listDirect (char *cwd, char *argpath)
char * listDirect (char * curloc){
  char * directory;
  DIR *dp;  //directory pointer
  struct dirent *ep;

  //if (check_dir_exist (
  errno = 0;
  dp = opendir(curloc);

  if(dp == NULL){
    fprintf(stderr, "Error: %s\n", strerror(errno));
  }

  while( (ep = readdir(dp)) ){
    if(ep->d_name[0] != '.'){
      char * pathNfile = malloc(strlen(curloc) + strlen(ep->d_name) + 2);
      strcpy(pathNfile, curloc);
      // printf("curloc: %s\n", curloc);
      strcat(pathNfile, "/");
      strcat(pathNfile, ep->d_name);
      // printf("file path: %s\n", pathNfile);
      printf("File Type: %u\n", ep->d_type);
      detailList(pathNfile);
      puts(ep->d_name);
    }

    //checks to see if its and error or eof
    if(ep == NULL){
      if(errno){
	fprintf(stderr, "Error: %s\n", strerror(errno));
	return NULL;
      }
    }
  }

  return directory;
}

void detailList(char * filepath){
  struct stat fileStat;
  int errchk;
  errno = 0;

  errchk = stat(filepath, &fileStat);

  // Check to see if stat() encountered any error
  if(errchk == -1){
    fprintf(stderr, "Error using stat function: %s\n", strerror(errno));
    return;
  }

  printf("Mode:                  %lo (octal)\n", (unsigned long) fileStat.st_mode);
}

char * changeDirect(char * curloc, char * directChanges){
  char newPath[MAX_FDATSZ];
  int changeSz;
  int curlocSz;

  if(directChanges[0] == '~'){
    return directChanges;
  }
  else if(directChanges[0] == '.'){
    if(directChanges[1] == '.'){
    }
    else{
    }
  }
  
  return newPath;
}
