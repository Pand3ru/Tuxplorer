#include<ncurses.h>

#include "draw.h"
#include "dirFunctions.h"

/* temporarily here */
struct dirContent **dir;

/* Function that creates a WINDOW* instance */
WINDOW *createWindow(int h, int w, int startx, int starty)
{
	WINDOW *local_win;
	local_win = newwin(h, w, starty, startx);
	wrefresh(local_win);
	return local_win;
}

/* Function copied from docs. Supposed to get rid of border */
void terminateWindow(WINDOW *window)
{
	box(window, ' ', ' ');
	wborder(window,  ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(window);
	wrefresh(window);
	delwin(window);
}

/* Function designed to draw a headline in the top-left of passed window */
void drawTopbar(WINDOW *w, char *text)
{
	wattron(w, A_REVERSE);
	mvwprintw(w, 1, 1, text);
	wattroff(w, A_REVERSE);

}

/* Needs refactoring ASAP */
void drawLayout(const char *path)
{
	WINDOW *rightPanelWindow, *leftPanelWindow, *footerWindow;
	int amt;

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

	dir = dirGetContent(path, &amt);

	printFolderMenu(rightPanelWindow, 0);

	box(leftPanelWindow, 0, 0);
	box(footerWindow, 0, 0);
	wrefresh(leftPanelWindow);
	wrefresh(footerWindow);

	int sel = 0;

	while(1)
	{
		char c = wgetch(rightPanelWindow);

		switch(c)
		{
			case 'j':
				sel = (sel + 1) % amt;
				break;
			case 'k':
				sel = (sel - 1 + amt) % amt;
				break;
			case '\n':
				wclear(footerWindow);
				path +=
				mvwprintw(footerWindow, 1, 1, "%s", dir[sel]->name);
			/*	if(dir[sel]->s == F_TRUE)
				{
					drawLayout(dir[sel]->name);
				}*/
			default:
				break;
		}
		printFolderMenu(rightPanelWindow,  sel%amt);
		box(leftPanelWindow, 0, 0);
		box(footerWindow, 0, 0);
		wrefresh(leftPanelWindow);
		wrefresh(footerWindow);
	}
	endwin();
}

/* Refactor this function in order to work for every window. Not just one specific one */
void printFolderMenu(WINDOW *w, int highlight)
{
	int count = 3;

	for(int i = 0; dir[i] != NULL; i++)
	{
		if(i == highlight)
		{
			wattron(w, A_REVERSE);
			mvwprintw(w, count, 1, "\t%s\n", dir[i]->name);
			wattroff(w, A_REVERSE);
		} else {
			if(dir[i]->s == F_TRUE)
			{
				wattron(w, A_BOLD);
				mvwprintw(w, count, 1, "\t%s\n", dir[i]->name);
				wattroff(w, A_BOLD);
			} else {
				mvwprintw(w, count, 1, "\t%s\n", dir[i]->name);
			}
		}
		count++;
	}
	box(w, 0, 0);
	wrefresh(w);
}
