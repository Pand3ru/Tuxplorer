#ifndef CONFIG_H
#define CONFIG_H

#include "dirFunctions.h"

void getConfigPath();
void setHomeDir();
void configPinFolder(struct dirContent *a);
int configRemovePin(struct dirContent *a);
void configGetValues();

#endif
