#ifndef __WARD_H__
#define __WARD_H__

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>

#ifndef BUFFER_SIZE 
#define BUFFER_SIZE 2048
#endif

int dcount(DIR* d); // returns number of dirs inside d
int fcount(DIR* d); // returns number of files inside d

char* offset_name(char* ppath, int offset, char** name_ptr);

char* dpath(char* ppath, char* direct);
char* ppath(char* dpath);
char* dir_list(char* dirname, char** dest);

#endif