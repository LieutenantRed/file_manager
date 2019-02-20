#include "dward.h"

int dcount(DIR* d) {
	int ret = 0;
	if (d != NULL) {
		rewinddir(d);
		struct dirent *dir;
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_DIR){
				++ret;
			}
		}
		rewinddir(d);
	}
	return ret;
}

int fcount(DIR* d) {
	int ret = 0;
	if (d != NULL) {
		rewinddir(d);
		struct dirent *dir;
		while ((dir = readdir(d)) != NULL) {
			++ret;
		}
		rewinddir(d);
	}
	return ret;
}

char* offset_name(char* ppath, int offset, char** name_ptr) {
	*name_ptr = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	strcpy(*name_ptr, "\0");
	DIR *d;
	struct dirent *dir;
	d = opendir(ppath);
	if (d) {
		int fc = fcount(d);
		for (int i = 0; (i < fc) && (i < offset); ++i) {
			dir = readdir(d);
		}
		strcat(*name_ptr, ppath);
		strcat(*name_ptr, dir->d_name);
	}
	closedir(d);
	return *name_ptr;
}


char* dpath(char* ppath, char* direct) { // перейти в direct директорию, NULL если не директория
	strncpy(ppath, direct, BUFFER_SIZE);
	strcat(ppath, "/");
	
	DIR *d;
	d = opendir(ppath);
	if (!d) {
		strcpy(ppath, "\0");
		return NULL;
	} 
	closedir(d);
	return ppath;
}

char* ppath(char* dpath) {
	if (dpath[0] == '\0')
		return dpath;
	char* iter;
	for (iter = dpath + strlen(dpath) - 2; (*iter != '/') && (iter >= dpath); --iter) ;
	*(++iter) = '\0';
	return dpath;
}

char* dir_list(char* dirname, char* dest) {
	*dest = '\0';
	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			strcat(dest, dir->d_name);
			strcat(dest, "\n");
		}
		dest[strlen(dest) - 1] = '\0';
	}
	closedir(d);
	return dest;
}

