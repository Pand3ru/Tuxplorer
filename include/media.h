#ifndef MEDIA_H
#define MEDIA_H

#include <ncurses.h>

bool isMediaFile(const char *filename);
void playMedia(char *filename);
void removeImage(WINDOW *w);

#endif
