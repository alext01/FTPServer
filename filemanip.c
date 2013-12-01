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
#include <inttypes.h>
#include <time.h>

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


int cmd_list (const char *cwd, const char *argpath, int detail)
{
  char *fullpath;

  //Determine if the file is a directory.
  if (!check_dir_exist (cwd, argpath))
    return false;

  //Create a single pathname to the directory from the pathname fragments.
  if ((fullpath = merge_paths (cwd, argpath, NULL)) == NULL)
    return false;
  
  listDirect (fullpath, detail);
  return 1;
}

char * listDirect (char * curloc, int detail){
  char * directory;
  DIR *dp;  //directory pointer
  struct dirent *ep;
  
  //if (check_dir_exist (
  errno = 0;
  dp = opendir(curloc);

  if(dp == NULL){
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return NULL;
  }

  int outSize = 4096;
  char * output;
  output = (char *)calloc(4096, sizeof(char));
  if(output == NULL){
    fprintf(stderr, "Error in allocating memory for output in list.");
    return NULL;
  }

  while( (ep = readdir(dp)) ){
    if(ep->d_name[0] != '.'){
      if(detail == 1){
	char * pathNfile = malloc(strlen(curloc) + strlen(ep->d_name) + 2);
	strcpy(pathNfile, curloc);
	strcat(pathNfile, "/");
	strcat(pathNfile, ep->d_name);

	detailList(ep, pathNfile, &output);
	free(pathNfile);
      }

      strcat(output, ep->d_name);
      if(strlen(output) >= (outSize-50)){
	outSize += 4096;
	output = (char *) realloc(output, outSize * sizeof(char));
      }
    }

    //send output

    free(output);

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


void detailList(struct dirent* dirInfo, char * filepath, char ** output){
  struct stat fileStat;
  int errchk;
  errno = 0;

  errchk = stat(filepath, &fileStat);

  // Check to see if stat() encountered any error
  if(errchk == -1){
    fprintf(stderr, "Error using stat function: %s\n", strerror(errno));
    return;
  }

  if(dirInfo->d_type == DT_DIR){
    strcat(*output, "d");
  }
  else if(dirInfo->d_type == DT_LNK){
    strcat(*output, "l");
  }
  else{
    strcat(*output, "-");
  }

  (fileStat.st_mode & S_IRUSR) ? strcat(*output,"r"):strcat(*output,"-");
  (fileStat.st_mode & S_IWUSR) ? strcat(*output,"w"):strcat(*output,"-");
  (fileStat.st_mode & S_IXUSR) ? strcat(*output,"x"):strcat(*output,"-");
  (fileStat.st_mode & S_IRGRP) ? strcat(*output,"r"):strcat(*output,"-");
  (fileStat.st_mode & S_IWGRP) ? strcat(*output,"w"):strcat(*output,"-");
  (fileStat.st_mode & S_IXGRP) ? strcat(*output,"x"):strcat(*output,"-");
  (fileStat.st_mode & S_IROTH) ? strcat(*output,"r"):strcat(*output,"-");
  (fileStat.st_mode & S_IWOTH) ? strcat(*output,"w"):strcat(*output,"-");
  (fileStat.st_mode & S_IXOTH) ? strcat(*output,"x  "):strcat(*output,"-  ");

  
  sprintf( (*output) + strlen(*output), "%zu   %d   %d   %lld   %s  ", fileStat.st_nlink, fileStat.st_uid, fileStat.st_gid, (unsigned long long)fileStat.st_size, ctime(&fileStat.st_mtime) );
  // printf("Mode:                  %lo (octal)\n", (unsigned long) fileStat.st_mode);

}


int makeDir(char * filepath){
  mode_t permissions;
  permissions = permissions | S_IRUSR;
  permissions = permissions | S_IWUSR;
  permissions = permissions | S_IRGRP;
  permissions = permissions | S_IROTH;

  errno = 0;

  if( (mkdir(filepath, permissions)) == -1){
    fprintf(stderr, "Error encountered while making directory: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}

/*
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
  }*/
