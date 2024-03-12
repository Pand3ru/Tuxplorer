#ifndef GLOBALS_H
#define GLOBALS_H

#include<ncurses.h>

extern WINDOW *rightPanelWindow;
extern WINDOW *leftPanelWindow;
extern WINDOW *footerWindow;

extern struct dirContent **dir;

extern int ignoreInvis;

extern char* textEditor;

#endif
