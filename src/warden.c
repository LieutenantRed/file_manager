#include "dward.h"

char* dir_list(char* dirname, char** dest) {
	*dest = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	**dest = '\0';
	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			strcat(*dest, dir->d_name);
			strcat(*dest, "\n");
		}
		(*dest)[strlen(*dest) - 1] = '\0';
	}
	closedir(d);
	return *dest;
}

char* dir_content(char* dirname, char** dest, int active_cell) {
	char* path = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	*path = '\0';
	strcat(path, dirname);
	strcat(path, "/");
	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);
	if (d) {
		for (int i = 0; i < active_cell; ++i) {
			dir = readdir(d);
		}
		if (dir->d_type == DT_DIR)
			strcat(path, dir->d_name);
		else
			*path = '\0';
		closedir(d);
		*dest = dir_list(path, dest);
	}
	free(path);
	return *dest;
}

char* here_i_go(char* dirname, char** dest, int active_cell) {
	*dest = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	**dest = '\0';
	strcat(*dest, dirname);
	strcat(*dest, "/");
	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);
	if (d) {
		for (int i = 0; i < active_cell; ++i) {
			dir = readdir(d);
		}
		strcat(*dest, dir->d_name);
		closedir(d);
	}
	return *dest;
}