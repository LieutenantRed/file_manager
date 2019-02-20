#include "graphics.h"

int init_display () {
	initscr();
	noecho();
	keypad(stdscr, TRUE);

	//initial colors
	if (has_colors() == FALSE) {
    	endwin();
	    fprintf(stderr, "Your terminal does not support color\n");
	    return DERR;
	}
	start_color();
	init_pair(CURRENT_ITEM, COLOR_CYAN, COLOR_BLACK);
	init_pair(ACTIVE_ITEM, COLOR_WHITE, COLOR_CYAN);
	assume_default_colors(COLOR_CYAN, COLOR_BLACK);

	//marks
	params.window_xs = COLS / 2;
	params.window_ys = LINES - 1; 

	current = newwin(params.window_ys, params.window_xs, 0, 0);
	next = newwin(params.window_ys, params.window_xs, 0, params.window_xs );
	sys = newwin(1, COLS, LINES - 1, 0);

	active_cell.win = current;
	curs_set(0); //hide cursor

	refresh();
	return 0;
}

void draw_borders(WINDOW* win) {
	box(win, 0 , 0);
	wrefresh(win);
}

void destroy_win(WINDOW* win) {
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);
}

void end_display() {
	destroy_win(current);
	destroy_win(next);
	free(active_cell.name);

	endwin();
}

// highlight active cell, returns number of lines
// drawing borders
int display_ls(WINDOW* win, char* list) {
	if (*list == '\0') {
		strcpy(list, "not accessable\0");
	}
	werase(win);
	//borders
	if ((BORDERS_ENABLED) && (win == current)) {
		wattron(win, A_BOLD | COLOR_PAIR(CURRENT_ITEM));
		draw_borders(win);
	}
	int x, y;
	x = 1; y = 1;
	int max_x = getmaxx(win);
	while (1) {
		if ((win == active_cell.win) && (y == active_cell.line)) {
			wattron(win, A_BOLD | COLOR_PAIR(ACTIVE_ITEM));
		} else {
			wattroff(win, A_BOLD);
			wattron(win, COLOR_PAIR(CURRENT_ITEM));
		}

		while ((*list != '\n') && (*list != '\0')) {
			mvwaddch(win, y, x, *list);
			++list;
			++x;
		}
		while (x < max_x - 1) {
			mvwaddch(win, y, x, ' ');
			++x;
		}
		++y;
		if (*list == '\0') {
			break;
		} else {
			x = 1;
			++list;
		}
	}
	wrefresh(win);
	return y;
}

void update_sysinfo(char* info) {
	werase(sys);
	wattron(sys, A_DIM | COLOR_PAIR(CURRENT_ITEM));
	waddstr(sys, info);
	wrefresh(sys);
}

// @_@ 
char* navigate(char* directory, char* response) {
	int ch;	// for switch only

	char* current_dir = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	strncpy(current_dir, directory, BUFFER_SIZE);
	char* next_dir = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	strncpy(next_dir, directory, BUFFER_SIZE);

	offset_name(directory, active_cell.line, &active_cell.name); 
	dpath(next_dir, active_cell.name);

	//panels filling here
	char* navigation_panel = (char*)malloc(BUFFER_SIZE * sizeof(char));
	char* right_panel = (char*)malloc(BUFFER_SIZE * sizeof(char));;
	dir_list(current_dir, navigation_panel);
	dir_list(next_dir, right_panel);
	int down_limit = display_ls(current, navigation_panel);
	display_ls(next, right_panel);

	// menu here
	while ((ch = getch())!='q') {
		switch(ch) {
		case 'w':
		case KEY_UP:
			if (active_cell.line > 1)
				--active_cell.line; // change current cell
			
			memset(next_dir, 0, BUFFER_SIZE);
			strncpy(next_dir, current_dir, BUFFER_SIZE);
			active_cell.name = offset_name(current_dir, active_cell.line, &active_cell.name);
			dpath(next_dir, active_cell.name);
			dir_list(next_dir, right_panel); //upload current cell content

			display_ls(next, right_panel); //display current cell content
			display_ls(current, navigation_panel); //display cursor highlight

			break;

		case 's':
		case KEY_DOWN: //KEY_DOWN
			if (active_cell.line < down_limit - 1) 
				++active_cell.line;
			
			memset(next_dir, 0, BUFFER_SIZE);
			strncpy(next_dir, current_dir, BUFFER_SIZE);
			active_cell.name = offset_name(current_dir, active_cell.line, &active_cell.name);
			dpath(next_dir, active_cell.name);
			dir_list(next_dir, right_panel); //upload current cell content

			display_ls(next, right_panel); //display current cell content
			display_ls(current, navigation_panel); //display cursor highlight
			break;

		case KEY_RESIZE: // #_#
			erase();
			init_display();
			display_ls(current, navigation_panel);
			display_ls(next, right_panel);
			break;

		case KEY_LEFT: ;//return back
			active_cell.line = 1;
			free(navigation_panel);
			free(right_panel);
			free(next_dir);
			strncpy(response, current_dir, BUFFER_SIZE);
			strcat(response, "/../");
			free(current_dir);
			return response;

		case '\n': 
		case KEY_RIGHT: ;
			active_cell.line = 1;
			if (next_dir[0] == '\0') {
				strncpy(response, current_dir, BUFFER_SIZE);
			} else {
				strncpy(response, next_dir, BUFFER_SIZE);
			}
			free(navigation_panel);
			free(right_panel);
			free(current_dir);
			free(next_dir);
			return response;
		}
	}
	free(navigation_panel);
	free(right_panel);
	free(current_dir);
	free(next_dir);
	strcpy(response, "exit");
	return response;
}

void display_navigation(char* path) {
	active_cell.line = 1;
	ppath(path);

	char* response = (char*)malloc(sizeof(char) * BUFFER_SIZE);
	while (strncmp(path, "exit", BUFFER_SIZE) != 0) {
		navigate(path, response);
		strncpy(path, response, BUFFER_SIZE); // update current state
	}
	free(response);
}