#include<ncurses.h>

#include "draw.h"
#include "dirFunctions.h"

WINDOW *createWindow(int h, int w, int startx, int starty)
{
	WINDOW *local_win;
	local_win = newwin(h, w, starty, startx);
	wrefresh(local_win);
	return local_win;
}

void terminateWindow(WINDOW *window)
{
	box(window, ' ', ' ');
	wborder(window,  ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(window);
	wrefresh(window);
	delwin(window);
}
void drawTopbar(WINDOW *w, char *text)
{
	wattron(w, A_REVERSE);
	mvwprintw(w, 1, 1, text);
	wattroff(w, A_REVERSE);

}

void drawLayout(const char *path)
{
	WINDOW *rightPanelWindow, *leftPanelWindow, *footerWindow;

	int count = 3;

	initscr();
	noecho();
	cbreak();
	curs_set(0);

	refresh();
	
	rightPanelWindow = createWindow(LINES-3, COLS*0.7, COLS*0.3, 0);
	leftPanelWindow = createWindow(LINES-3, COLS*0.3, 0, 0);
	footerWindow = createWindow(3, COLS-1, 0, LINES-3);

	drawTopbar(rightPanelWindow, "PWD");
	drawTopbar(leftPanelWindow, "Cool Folders");
	drawTopbar(footerWindow, "Explain Vim Motions to me please uwu");

	struct dirContent **dir;

	dir = dirGetContent(path);

	for(int i = 0; dir[i] != NULL; i++)
	{
		if(dir[i]->s == F_TRUE)
		{
			wattron(rightPanelWindow, A_BOLD);
			mvwprintw(rightPanelWindow, count, 1, "\t%s\n", dir[i]->name);
			wattroff(rightPanelWindow, A_BOLD);
		} else {
			mvwprintw(rightPanelWindow, count, 1, "\t%s\n", dir[i]->name);
		}
		count++;
	}
	box(rightPanelWindow, 0, 0);
	box(leftPanelWindow, 0, 0);
	box(footerWindow, 0, 0);
	wrefresh(rightPanelWindow);
	wrefresh(leftPanelWindow);
	wrefresh(footerWindow);
	getch();
	terminateWindow(rightPanelWindow);
	terminateWindow(leftPanelWindow);
	terminateWindow(footerWindow);
	printw("I am done");
	getch();
	endwin();
}


