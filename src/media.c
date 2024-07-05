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

void calculate_new_dimensions(int original_width, int original_height,
                              int *new_width, int *new_height, int max_width,
                              int max_height) {
  float aspect_ratio = (float)original_width / (float)original_height;

  if (original_width > max_width || original_height > max_height) {
    if ((float)max_width / (float)max_height > aspect_ratio) {
      *new_height = max_height;
      *new_width = (int)(max_height * aspect_ratio);
    } else {
      *new_width = max_width;
      *new_height = (int)(max_width / aspect_ratio);
    }
  } else {
    *new_width = original_width;
    *new_height = original_height;
  }
}

int getImageDimensions(const char *filename, int *width, int *height) {
  int original_width, original_height, n;
  unsigned char *data =
      stbi_load(filename, &original_width, &original_height, &n, 0);
  if (data) {
    stbi_image_free(data);
    printf("Original Width: %d, Original Height: %d\n", original_width,
           original_height); // Debug
    calculate_new_dimensions(original_width, original_height, width, height,
                             COLS * 18, LINES * 8);
    printf("New Width: %d, New Height: %d\n", *width, *height);
    sleep(3);
    return 0;
  } else {
    return -1;
  }
}

int displayImage(const char *filename, int new_width, int new_height,
                 int startx, int starty) {
  char string_width[128], string_height[128];
  sprintf(string_width, "%d", new_width);
  sprintf(string_height, "%d", new_height);

  printf("HERE: %s, %s\nAND: %d, %d\n", string_width, string_height, startx,
         starty);
  sleep(3);

  const char *term = getenv("TERM");
  if (term == NULL) {
    fprintf(stderr, "Cannot determine terminal type.\n");
    return EXIT_FAILURE;
  }

  if (strcmp(term, "xterm") == 0 || strcmp(term, "xterm-256color") == 0) {
    fprintf(stdout, "\033[?80h"); // Enable SIXEL mode in xterm
  } else if (strcmp(term, "xterm-kitty") == 0) {
    char command[256];
    snprintf(command, sizeof(command),
             "kitty +kitten icat --scale-up --place=%dx%d@%dx%d %s",
             new_width / 8, new_height / 3, startx, starty, filename);
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
  sixel_encoder_t *encoder;

  status = sixel_encoder_new(&encoder, NULL);
  if (SIXEL_FAILED(status)) {
    return EXIT_FAILURE;
  }

  // Set the width and height for the sixel encoder
  status = sixel_encoder_setopt(encoder, SIXEL_OPTFLAG_WIDTH, string_width);
  if (SIXEL_FAILED(status)) {
    sixel_encoder_unref(encoder);
    return EXIT_FAILURE;
  }

  status = sixel_encoder_setopt(encoder, SIXEL_OPTFLAG_HEIGHT, string_height);
  if (SIXEL_FAILED(status)) {
    sixel_encoder_unref(encoder);
    return EXIT_FAILURE;
  }

  status = sixel_encoder_encode(encoder, filename);
  if (SIXEL_FAILED(status)) {
    sixel_encoder_unref(encoder);
    return EXIT_FAILURE;
  }

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
  int outx, outy;
  WINDOW *w = createMediaWindow(imageW, imageH, &outx, &outy);

  drawMediaBorder(w);

  // Render the image
  if (displayImage(filename, imageW, imageH, outx, outy) != 0) {
    fprintf(stderr, "Error displaying image.\n");
    return;
  }

  handleMediaInput(w);
}
void removeImage(WINDOW *w) {
  wclear(w);
  box(w, 0, 0);
  wrefresh(w);

  const char *term = getenv("TERM");
  if (term != NULL &&
      (strcmp(term, "xterm") == 0 || strcmp(term, "xterm-256color") == 0)) {
    fprintf(stdout, "\033[2J\033[H"); // Clear the screen and move the cursor to
                                      // the home position
  } else if (term != NULL && strcmp(term, "xterm-kitty") == 0) {
    char command[256];
    snprintf(command, sizeof(command), "kitty +kitten icat --clear");
    system(command);
  }
}
