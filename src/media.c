#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include <ncurses.h>
#include <sixel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>

#include "../include/controls.h"
#include "../include/draw.h"
#include "../include/globals.h"
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

int getImageDimensions(const char *filename, int *width, int *height) {
  int n;
  unsigned char *data = stbi_load(filename, width, height, &n, 0);
  if (data) {
    stbi_image_free(data);
    return 0;
  } else {
    return -1;
  }
}

// Function to display image using sixel
int displayImage(const char *filename) {
  // Get the terminal type from the environment
  const char *term = getenv("TERM");
  if (term == NULL) {
    fprintf(stderr, "Cannot determine terminal type.\n");
    return EXIT_FAILURE;
  }

  if (strcmp(term, "xterm") == 0 || strcmp(term, "xterm-256color") == 0) {
    // Ensure xterm has proper SIXEL support
    fprintf(stdout, "\033[?80h"); // Enable SIXEL mode in xterm
  } else if (strcmp(term, "xterm-kitty") == 0) {
    // Use kitty's icat tool
    char command[256];
    snprintf(command, sizeof(command), "kitty +kitten icat %s", filename);
    system(command);
    return EXIT_SUCCESS;
  } else if (strcmp(term, "konsole") == 0) {
    // Assume konsole works with SIXEL
  } else {
    fprintf(stderr, "Terminal type %s not supported for SIXEL graphics.\n",
            term);
    return EXIT_FAILURE;
  }

  // Initialize SIXEL status variable
  SIXELSTATUS status;

  // Initialize sixel encoder
  sixel_encoder_t *encoder;
  status = sixel_encoder_new(&encoder, NULL);
  if (SIXEL_FAILED(status)) {
    return EXIT_FAILURE;
  }

  // Load and encode the image from a file
  status = sixel_encoder_encode(encoder, filename);
  if (SIXEL_FAILED(status)) {
    sixel_encoder_unref(encoder);
    return EXIT_FAILURE;
  }

  // Cleanup
  sixel_encoder_unref(encoder);
  return EXIT_SUCCESS;
}

void playMedia(char *filename) {
  int imageW, imageH;

  // Get image dimensions
  if (getImageDimensions(filename, &imageW, &imageH) != 0) {
    fprintf(stderr, "Error getting image dimensions.\n");
    return;
  }
  // Create media window based on image dimensions
  WINDOW *w = createMediaWindow(imageW, imageH);

  char s[50];

  sprintf(s, "SIZE: %d, %d", imageW, imageH);

  drawTopbar(w, s);
  drawMediaBorder(w);

  // Render the image
  if (displayImage(filename) != 0) {
    fprintf(stderr, "Error displaying image.\n");
    return;
  }

  handleMediaInput(w);
}
void removeImage(WINDOW *w) {
  wclear(w);    // Clear the window content
  box(w, 0, 0); // Redraw the border
  wrefresh(w);  // Refresh the window to update display

  // Reset the terminal display mode to clear the SIXEL image
  const char *term = getenv("TERM");
  if (term != NULL &&
      (strcmp(term, "xterm") == 0 || strcmp(term, "xterm-256color") == 0)) {
    fprintf(stdout, "\033[2J\033[H"); // Clear the screen and move the cursor to
                                      // the home position
  } else if (term != NULL && strcmp(term, "xterm-kitty") == 0) {
    // kitty terminal handling if needed
    char command[256];
    snprintf(command, sizeof(command), "kitty +kitten icat --clear");
    system(command);
  }
}
