#include<ncurses.h>

#include "../include/draw.h"
#include "../include/dirFunctions.h"
#include "../include/controls.h"
#include "../include/globals.h"

/* temporarily here */
struct dirContent **dir = NULL;

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
	mvwprintw(w, 1, 1, "%s", text);
	wattroff(w, A_REVERSE);

}

void initScreen()
{
	initscr();
	start_color();
	noecho();
	cbreak();
	curs_set(0);
	refresh();
}

void createWindows()
{
	rightPanelWindow = createWindow(LINES-3, COLS*0.7, COLS*0.3, 0);
	leftPanelWindow = createWindow(LINES-3, COLS*0.3, 0, 0);
	footerWindow = createWindow(3, COLS-1, 0, LINES-3);
}

void drawTopbars()
{
	drawTopbar(rightPanelWindow, "PWD");
	drawTopbar(leftPanelWindow, "Cool Folders");
	drawTopbar(footerWindow, "[j] down\t[k] up\t[i] toggle hidden content\t[r] rename\t[d] delete file\t[c] create file\t[q] quit");
}

void drawBorders()
{

	box(rightPanelWindow, 0, 0);
	box(leftPanelWindow, 0, 0);
	box(footerWindow, 0, 0);
	wrefresh(rightPanelWindow);
	wrefresh(leftPanelWindow);
	wrefresh(footerWindow);
}
/* Function that draws the basic Layout */
void drawLayout()
{
	initScreen();
	createWindows();
	drawTopbars();
	drawBorders();
	/* Here is the part where I have to insert window switching */
	ctrlFolderView();
}

/* Refactor this function in order to work for every window. Not just one specific one */
void printFolderMenu(WINDOW *w, int highlight, int start, int offset, struct dirContent **dire)
{
	int count = offset;

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

	for(int i = start; dire[i] != NULL; i++)
	{
		if(i == highlight)
		{
			wattron(w, A_REVERSE);
			wattron(w, COLOR_PAIR(1));
			mvwprintw(w, count, 1, "\t%s\n", dire[i]->name);
			wattroff(w, A_REVERSE);
			wattroff(w, COLOR_PAIR(1));
		} else {
			if(dir[i]->s == F_TRUE)
			{
				wattron(w, A_BOLD);
				wattron(w, COLOR_PAIR(2));
				mvwprintw(w, count, 1, "\t%s\n", dire[i]->name);
				wattroff(w, COLOR_PAIR(2));
				wattroff(w, A_BOLD);
			} else {
				mvwprintw(w, count, 1, "\t%s\n", dire[i]->name);
			}
		}
		count++;
	}
	drawBorders();
}


