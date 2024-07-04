#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>

extern WINDOW *rightPanelWindow;
extern WINDOW *leftPanelWindow;
extern WINDOW *footerWindow;

extern struct dirContent **dir;
extern struct dirContent **pins;

extern int ignoreInvis;

extern int isInWindowSelection;

extern int selectedWindow;

extern char *textEditor;

extern char *homeDir;
extern char *configFile;

extern int amount_pins;

extern int amount_folder;

#endif
