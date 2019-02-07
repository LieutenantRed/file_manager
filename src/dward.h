#ifndef __WARD_H__
#define __WARD_H__

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>

#ifndef BUFFER_SIZE 
#define BUFFER_SIZE 4000
#endif

char* dir_list(char* dirname, char** dest);
char* dir_content(char* dirname, char** dest, int active_cell);
char* here_i_go(char* dirname, char** dest, int active_cell);
#endif