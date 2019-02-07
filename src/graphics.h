#ifndef __GRAPHX_H__
#define __GRAPHX_H__

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ncurses.h>

//string
#ifndef BUFFER_SIZE 
#define BUFFER_SIZE 1024
#endif

//colors
#define DEFAULT_ITEM 1
#define CURRENT_ITEM 2
#define ACTIVE_ITEM  3
#define BORDERS		 4

#define DERR -33
#define BORDERS_ENABLED 1

WINDOW *current, *prev, *next, *sys;

struct {
	size_t window_xs, window_ys;
	int lvl;
} params;

struct {
	WINDOW* win;
	int line;
} active_cell;

// initial fnc
int init_display();
void end_display();



void display_all();
#endif