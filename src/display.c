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
	init_pair(CURRENT_ITEM, COLOR_RED, COLOR_BLACK);
	init_pair(ACTIVE_ITEM, COLOR_WHITE, COLOR_RED);
	assume_default_colors(COLOR_RED, COLOR_BLACK);

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

	endwin();
}

// highlight active cell, returns number of lines
// drawing borders
int display_ls(WINDOW* win, char* list) {
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
int key_handling(char* begdir) {
	int ch;

	//panels filling here
	char* navigation_panel;
	char* right_panel;
	dir_list(begdir, &navigation_panel);
	dir_content(begdir, &right_panel, active_cell.line);
	int down_limit = display_ls(current, navigation_panel);
	display_ls(next, right_panel);

	// menu here
	while ((ch = getch())!='q') {
		switch(ch) {
		case KEY_UP:
			if (active_cell.line > 1)
				--active_cell.line;
			display_ls(current, navigation_panel); //navigate cursor highlight
			dir_content(begdir, &right_panel, active_cell.line); //upload current cell content
			display_ls(next, right_panel); //display current cell content
			break;

		case KEY_DOWN: //KEY_UP
			if (active_cell.line < down_limit - 1) 
				++active_cell.line;
			display_ls(current, navigation_panel); 
			dir_content(begdir, &right_panel, active_cell.line);
			display_ls(next, right_panel);
			break;

		case KEY_RESIZE: // #_#
			erase();
			init_display();
			display_ls(current, navigation_panel);
			display_ls(next, right_panel);
			break;

		case KEY_LEFT: //return back
			free(navigation_panel);
			free(right_panel);
			return 1;

		case KEY_ENTER:
		case KEY_RIGHT: ;
			char* way_through;
			here_i_go(begdir, &way_through, active_cell.line);
			int ret = key_handling(way_through);
			if (ret == 0) {
				free(navigation_panel);
				free(right_panel);
				return 0;
			}
			display_ls(next, right_panel);
			display_ls(current, navigation_panel);
			free(way_through);

			break;
		}
	}
	free(navigation_panel);
	free(right_panel);
	return 0;
}

void display_all() {
	active_cell.line = 1;
	while (key_handling(".") != 0) ;
}

