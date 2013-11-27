#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
//#include <ctype.h>
//#include <unistd.h>

#include "filemanip.h"
#include "common.h"

#define MAX_FDATSZ 4096

extern int verbflg;
extern byte_t fileBuff[MAX_FDATSZ];

FILE * openFile(char * fileName, char * purp){
  FILE *fp;

  if((fp = fopen(fileName, purp)) == NULL)
    {
      if(verbflg)
	fprintf(stderr, "Cannot open file: %s\n", fileName);
      else
	fprintf(stderr, "Error");
      exit(1);
    }

  printf("file open success\n");
  return fp;
}


long int fSzCount(FILE * fp){
  long int fileSZ = 0;
  int errck;

  if( (errck = fseek(fp, 0, SEEK_END)) == -1){
    fprintf(stderr, "Error: seeking EOF\n");
    exit(1);
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


char * listDirect(char * curloc){
  char * directory;
  DIR *dp;
  struct dirent *ep;

  dp = opendir(curloc);

  if(dp != NULL){
    while(ep = readdir(dp)){
      puts(ep->d_name);
    }

    //checks to see if its and error or eof
    if(errno){
      fprintf(stderr, "Error: %s\n", strerror(errno));
      exit(0);
    }
  }
  else{
    fprintf(stderr, "Error opening directory\n");
    return "err";
  }

  return directory;
}
