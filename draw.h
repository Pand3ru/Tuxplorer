#ifndef DRAW_H
#define DRAW_H

#include<ncurses.h>

WINDOW* createWindow(int height, int width, int startx, int starty);
void terminateWindow(WINDOW *w);
void drawTopbar(WINDOW *w, char *text);
void drawLayout(const char *path);

#endif
