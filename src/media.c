#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>

#include "../include/controls.h"
#include "../include/draw.h"
#include "../include/media.h"

bool isMediaFile(const char *filename) {
  const char *media_extensions[] = {".png", ".jpg", ".jpeg",
                                    ".gif", ".mp4", ".avi"};
  int num_media_extensions =
      sizeof(media_extensions) / sizeof(media_extensions[0]);
  const char *extension = strrchr(filename, '.');
  if (extension != NULL) {
    for (int i = 0; i < num_media_extensions; ++i) {
      if (strcmp(extension, media_extensions[i]) == 0) {
        return true;
      }
    }
  }
  return false;
}

void playMedia(char *filename) {
  WINDOW *w = createMediaWindow(900, 600);
  drawMediaBorder(w);
  handleMediaInput(w);
}
