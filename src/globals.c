#include "../include/globals.h"

WINDOW *rightPanelWindow = NULL;
WINDOW *leftPanelWindow = NULL;
WINDOW *footerWindow = NULL;

int ignoreInvis = 1;

int isInWindowSelection = 1;

int selectedWindow = 1; // 1 is folderview (right Panel) 0 is Panelview (left Panel)

char *textEditor = "vim"; // Here to prevent sefaults

char *homeDir = NULL;
char *configFile = NULL;

struct dirContent **dir = NULL;
struct dirContent **pins = NULL;

int amount_pins;
int amount_folder;
