#ifndef DRAW_H
#define DRAW_H

#include "../include/dirFunctions.h"

#include<ncurses.h>

WINDOW* createWindow(int height, int width, int startx, int starty);
void terminateWindow(WINDOW *w);
void drawTopbar(WINDOW *w, char *text);
void drawLayout();
void initScreen();
void createWindows();
void drawTopbars();
void printFolderMenu(WINDOW *w, int highlight, int start, int offset, struct dirContent **dire);

#endif
