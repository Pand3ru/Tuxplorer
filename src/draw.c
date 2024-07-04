#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#include <ncurses.h>
#include <unistd.h>

#include "../include/config.h"
#include "../include/controls.h"
#include "../include/dirFunctions.h"
#include "../include/draw.h"
#include "../include/globals.h"

/* Function that creates a WINDOW* instance */
WINDOW *createWindow(int h, int w, int startx, int starty) {
  WINDOW *local_win;
  local_win = newwin(h, w, starty, startx);
  wrefresh(local_win);
  return local_win;
}

/* Function copied from docs. Supposed to get rid of border */
void terminateWindow(WINDOW *window) {
  box(window, ' ', ' ');
  wborder(window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wclear(window);
  wrefresh(window);
  delwin(window);
}

/* Function designed to draw a headline in the top-left of passed window */
void drawTopbar(WINDOW *w, char *text) {
  wattron(w, A_REVERSE);
  mvwprintw(w, 1, 1, "%s", text);
  wattroff(w, A_REVERSE);
}

void initScreen() {
  setHomeDir();
  getConfigPath();
  configGetValues();

  initscr();
  if (!stdscr) {
    fprintf(stderr, "Error initializing ncurses\n");
    return;
  }
  start_color();
  keypad(stdscr, TRUE);
  noecho();
  cbreak();
  curs_set(0);
  refresh();
}

void createWindows() {
  rightPanelWindow = createWindow(LINES - 3, COLS * 0.7, COLS * 0.3, 0);
  leftPanelWindow = createWindow(LINES - 3, COLS * 0.3, 0, 0);
  footerWindow = createWindow(3, COLS - 1, 0, LINES - 3);
}

void drawTopbars() {
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));

  drawTopbar(rightPanelWindow, cwd);
  drawTopbar(leftPanelWindow, "Pinned Folders");

  if (!isInWindowSelection && selectedWindow == 1) {
    drawTopbar(footerWindow, "[j] down\t[k] up\t[i] toggle hidden content\t[r] "
                             "rename\t[d] delete file\t[c] create file\t[p] "
                             "pin selected\t[ESC] window selection \t[q] quit");
  } else if (!isInWindowSelection && selectedWindow == 0) {
    drawTopbar(
        footerWindow,
        "[j] down\t[k] up\t[ESC] window selection\t[r] remove pin\t[q] quit");
  } else {
    drawTopbar(footerWindow, "[j] right\t[k] left\t[q] quit");
  }
}

void drawBorders() {
  box(rightPanelWindow, 0, 0);
  box(leftPanelWindow, 0, 0);
  box(footerWindow, 0, 0);
  wrefresh(rightPanelWindow);
  wrefresh(leftPanelWindow);
  wrefresh(footerWindow);
}

WINDOW *createMediaWindow(int imageW, int imageH) {
  int win_height = LINES - 5;
  int win_width = COLS * 0.8;
  int startx = (COLS * 0.5) - (win_width * 0.5);
  int starty = ((LINES - 3) * 0.5) - (win_height * 0.5);
  return createWindow(win_height, win_width, startx, starty);
}

void drawMediaBorder(WINDOW *w) {
  box(w, 0, 0);
  wrefresh(w);
}

/* Function that draws the basic Layout */
void drawLayout() {
  initScreen();
  createWindows();
  drawTopbars();
  drawBorders();

  if (isInWindowSelection) {
    windowSelector();
  } else {
    switch (selectedWindow) {
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

void printFolderMenu(WINDOW *w, int highlight, int start, int offset,
                     struct dirContent **dire, int amount) {
  int count = offset;

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

  for (int i = start; i < amount; i++) {
    if (i == highlight) {
      wattron(w, A_REVERSE);
      wattron(w, COLOR_PAIR(1));
      mvwprintw(w, count, 1, "\t%s\n", dire[i]->name);
      wattroff(w, A_REVERSE);
      wattroff(w, COLOR_PAIR(1));
    } else {
      if (dire[i]->s == F_TRUE) {
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

  if (selectedWindow == 1) {
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
