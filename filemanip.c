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
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>

#include "filemanip.h"
#include "net.h"
#include "path.h"
#include "response.h"
#include "session.h"

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

/*int closeFile(FILE *fp){
  int errchk;
  errno = 0;

  errchk = fclose(fp);
  if(errno){
    fprintf(stderr, "Error in function closeFile: %s\n", strerror(errno));
    return -1;
  }
  return 0;
  }*/

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


void cmd_list_nlst (session_info_t *si, const char *arg, bool detail)
{
  char *fullpath;

  char *noAccess;
  char *transferStart;
  char *noConnection;
  

 if (si->logged_in == false) {
    noAccess = "550 - Access denied.\n";
    send_all(si->c_sfd, (uint8_t *)noAccess, strlen(noAccess));
    return;
  }

  //Determine if the file is a directory.
  if (!check_dir_exist (si->cwd, arg)) {
    send_mesg_553 (si->c_sfd);
    return;
  }

  transferStart = "150 Here comes the directory listing.\n";
  send_all(si->c_sfd,(uint8_t*)transferStart,strlen(transferStart));

  if (si->d_sfd == 0) {
    noConnection = "425 - Cannot open data connection; please use the PORT or PASV command first.\n";
    send_all(si->c_sfd, (uint8_t *)noConnection, strlen(noConnection));
    return;
  }

  //Create a single pathname to the directory from the pathname fragments.
  if ((fullpath = merge_paths (si->cwd, arg, NULL)) == NULL) {
    send_mesg_451 (si->c_sfd);
    close (si->d_sfd);
    si->d_sfd = 0;
    return;
  }

  listDirect (si, fullpath, detail);
  free (fullpath);
  return;
}


void listDirect (session_info_t *si, char * fullpath, bool detail){
  //char * directory;
  DIR *dp;                       //directory pointer
  struct dirent *ep;             //entry pointer
  char *output;                  //output buffer
  int outSize = 4096;
  char *aborted;
  char *success;

  if ((dp = opendir(fullpath)) == NULL) {
    fprintf (stderr, "%s: opendir: %s\n", __FUNCTION__, strerror (errno));
    send_mesg_451 (si->c_sfd);
    close (si->d_sfd);
    si->d_sfd = 0;
    return;
  }

  output = calloc(outSize, sizeof(*output));
  if(output == NULL){
    fprintf(stderr, "Error in allocating memory for output in list.");
    send_mesg_451 (si->c_sfd);
    close (si->d_sfd);
    si->d_sfd = 0;
    return;
  }

  errno = 0;
  while(((ep = readdir(dp)) != NULL) && (si->cmd_abort == false)){
    //Do not list hidden files, current directory, or parent directory.
    if(ep->d_name[0] != '.'){
      if(detail == true){
	detailList(ep, fullpath, &output);
      } else {
	strcat(output, ep->d_name);
	strcat(output, "\r\n");
      }

      if(strlen(output) >= (outSize-1000)){
      	outSize += 4096;
      	if ((output = realloc(output, outSize * sizeof (*output))) == NULL) {
	  fprintf (stderr, "%s: realloc: %s\n", __FUNCTION__,
		   "could not allocate the required space");
	  send_mesg_451 (si->c_sfd);
	  close (si->d_sfd);
	  si->d_sfd = 0;
	  return;
	}
      }
    }
  }

  //checks to see if its and error or eof
  if(ep == NULL){
    if(errno){
      fprintf(stderr, "%s: readdir: %s\n", __FUNCTION__, strerror(errno));
      send_mesg_451 (si->c_sfd);
      close (si->d_sfd);
      si->d_sfd = 0;
      closedir (dp);
      return;
    }
  }

  send_all (si->d_sfd, (uint8_t*)output, strlen (output));
  free (output);

  if (si->cmd_abort == true) {
    aborted = "426 - Connection close; transfer aborted.\n";
    send_all(si->c_sfd, (uint8_t *)aborted, strlen(aborted));
    si->cmd_abort = false;
  } else {
    success = "226 - Closing data connection; requested file action successful.\n";
    send_all(si->c_sfd, (uint8_t *)success, strlen(success));
  }

  if (closedir (dp) == -1)
    fprintf (stderr, "%s: closedir: %s\n", __FUNCTION__, strerror (errno));
  
  close (si->d_sfd);
  si->d_sfd = 0;
  return;
}


void detailList(struct dirent* dirInfo, char * fullpath, char ** output){
  char time[20];
  struct stat fileStat;
  int errchk;
  struct tm * timeinfo;
  errno = 0;

  char filename[strlen (fullpath) + 256];
  filename[0] = '\0';
  strcat (filename, fullpath);
  strcat (filename, dirInfo->d_name);

  errchk = stat(filename, &fileStat);

  // Check to see if stat() encountered any error
  if(errchk == -1){
    fprintf(stderr, "Error using stat function: %s\n", strerror(errno));
    return;
  }

  if ((timeinfo = gmtime (&fileStat.st_mtime)) == NULL) {
    fprintf (stderr, "%s: gmtime: error with function call\n", __FUNCTION__);
    return;
  }
  strftime (time, 20, "%b %d %Y", timeinfo);

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
  (fileStat.st_mode & S_IXOTH) ? strcat(*output,"x"):strcat(*output,"-");

  
  sprintf( (*output) + strlen(*output), " %zu   %d   %d   %lld   %s   %s\r\n",
	   fileStat.st_nlink, fileStat.st_uid, fileStat.st_gid,
	   (unsigned long long)fileStat.st_size, time, dirInfo->d_name);
  // printf("Mode:                  %lo (octal)\n", (unsigned long) fileStat.st_mode);
  return;
}


int makeDir(session_info_t *si, char * filepath){
  mode_t permissions = 0;
  permissions = permissions | S_IRUSR;
  permissions = permissions | S_IWUSR;
  permissions = permissions | S_IRGRP;

  errno = 0;

  if( (mkdir(filepath, permissions)) == -1){
    fprintf(stderr, "%s: mkdir: %s\n", __FUNCTION__, strerror(errno));
    
    send_mesg_550(si->c_sfd);
    close(si->d_sfd);
    si->d_sfd = 0;

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
