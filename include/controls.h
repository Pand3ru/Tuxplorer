#ifndef CONTROLS_H
#define CONTROLS_H

#include <ncurses.h>

void ctrlFolderView();
void ctrlPinView();
int promtConfirmation();
void promtMakeInputReady();
void promtGetInput(char *input, char *text);
void windowSelector();
void terminateApp();
void openInTexteditor(int sel);
void handleMediaInput(WINDOW *w);

#endif
