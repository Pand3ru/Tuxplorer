#include<ncurses.h>

#include "../include/draw.h"
#include "../include/dirFunctions.h"
#include "../include/controls.h"
#include "../include/globals.h"
#include "../include/config.h"

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
	setHomeDir();
	getConfigPath();

	initscr();
	start_color();
	keypad(stdscr, TRUE);
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

	if(!isInWindowSelection && selectedWindow == 1)
	{
	      drawTopbar(footerWindow, "[j] down\t[k] up\t[i] toggle hidden content\t[r] rename\t[d] delete file\t[c] create file\t[ESC] window selection \t[q] quit");
	} else {
		drawTopbar(footerWindow, "[j] down\t[k] up\t[ESC] window selection \t[q] quit");
	}
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

	if(isInWindowSelection)
	{
		windowSelector();
	} else {
		switch(selectedWindow)
		{
			case 1:
				ctrlFolderView();
				return;
			case 0:
				ctrlPinView();
				return;
			default:
				return;
		}
	}
	
}

void printFolderMenu(WINDOW *w, int highlight, int start, int offset, struct dirContent **dire, int amount)
{
	int count = offset;

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

	for(int i = start; i < amount; i++)
	{
		if(i == highlight)
		{
			wattron(w, A_REVERSE);
			wattron(w, COLOR_PAIR(1));
			mvwprintw(w, count, 1, "\t%s\n", dire[i]->name);
			wattroff(w, A_REVERSE);
			wattroff(w, COLOR_PAIR(1)); 
		} else {
			if(dire[i]->s == F_TRUE)
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

	if(selectedWindow == 1)
	{
		wattron(rightPanelWindow, COLOR_PAIR(2));
		box(rightPanelWindow, 0, 0);
		wattroff(rightPanelWindow, COLOR_PAIR(2));
		wrefresh(rightPanelWindow);
	} else {
		wattron(leftPanelWindow, COLOR_PAIR(2));
		box(leftPanelWindow, 0, 0);
		wattroff(leftPanelWindow, COLOR_PAIR(2));
		wrefresh(leftPanelWindow);
	}
}


