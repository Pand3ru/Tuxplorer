#include "../include/config.h"
#include "../include/globals.h"
#include<stdlib.h>
#include<string.h>

void getConfigPath() {
    int bufSize = 1024;
    char *filePath = malloc(bufSize);
    if (!filePath) {
        fprintf(stderr, "Fehler bei der Speicherzuweisung für Konfigurationspfad.\n");
        return;
    }

    snprintf(filePath, bufSize, "%s/.config/tuxplorer/config.cfg", homeDir);

    configFile = filePath;
}

void setHomeDir()
{
	homeDir = getenv("HOME");	
	if(homeDir == NULL)
	{
		homeDir = "Yikes.";
	}
}
