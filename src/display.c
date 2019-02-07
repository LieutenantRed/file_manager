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

	params.window_xs = COLS / 2; //window max x
	params.window_ys = LINES - 1; //window max y

	prev = newwin(params.window_ys, params.window_xs / 2, 0, 0);
	current = newwin(params.window_ys, params.window_xs, 0, params.window_xs / 2 );
	next = newwin(params.window_ys, params.window_xs, 0, params.window_xs / 2 +  params.window_xs);
	sys = newwin(1, COLS, LINES - 1, 0);

	active_cell.win = current;
	active_cell.line = 1;

	assume_default_colors(COLOR_RED, COLOR_BLACK);
	curs_set(0);

	refresh();
	return 0;
}

void draw_borders(WINDOW* win) {
	box(win, 0 , 0);
	wrefresh(win);
}

void clear_borders(WINDOW* win) {
	box(win, ' ' , ' ');
	wrefresh(win);
}

void destroy_win(WINDOW* win) {
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);
}

void end_display() {
	destroy_win(prev);
	destroy_win(current);
	destroy_win(next);

	endwin();
}

void display_ls(WINDOW* win, char* list) {
	werase(win);
	//borders
	if ((BORDERS_ENABLED) && (win == current))
		draw_borders(win);
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
}

void update_sysinfo(char* info) {
	werase(sys);
	wattron(sys, A_DIM | COLOR_PAIR(CURRENT_ITEM));
	waddstr(sys, info);
	wrefresh(sys);
}

void display_all() {
	params.lvl = 2; // lvl 2 ~ "/home/*username*", lvl 0 == "/" 

	
	display_ls(current, "sdsdsd\ndsfghjgf\ndsafsgdh\nasdsfg\nasdc\0");

	display_ls(current, "hello\ndarkness\0");
	update_sysinfo("my old friend");


}