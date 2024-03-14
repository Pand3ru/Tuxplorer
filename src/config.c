#include "../include/config.h"
#include "../include/globals.h"
#include "../include/dirFunctions.h"

#include<stdlib.h>
#include<string.h>
#include<libconfig.h>
#include<unistd.h>

void getConfigPath() {
    int bufSize = 1024;
    char *filePath = malloc(bufSize);
    if (!filePath) {
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

void configPinFolder(struct dirContent *a)
{
	config_t cfg;
	config_setting_t *group, *setting;
	chdir(a->name);
	char path[128];

	if(getcwd(path, sizeof(path)) == NULL)
	{
		chdir("..");
		return;	
	}
	chdir("..");


	config_init(&cfg);

	if(!config_read_file(&cfg, configFile))
	{
		config_destroy(&cfg);
		return;
	}

	group = config_lookup(&cfg, "pins");
	if(group == NULL) 
	{
		config_destroy(&cfg);
		return;
	}
	setting = config_setting_get_member(group, a->name);

	if (setting != NULL) 
	{
		return;
    	} else {
		setting = config_setting_add(group, a->name, CONFIG_TYPE_STRING);
        	config_setting_set_string(setting, path);
    	}

	config_write_file(&cfg, configFile);
	config_destroy(&cfg);
}

int configRemovePin(struct dirContent *a)
{
	config_t cfg;
	config_setting_t *group;

	config_init(&cfg);

	if(config_read_file(&cfg, configFile)) {
	    group = config_lookup(&cfg, "pins");
	    if(group != NULL) {
		config_setting_remove(group, a->name);
	    }
	    config_write_file(&cfg, configFile);
	}
	config_destroy(&cfg);
	return 1;
}

void configGetValues()
{
	int i = 0;

	config_t cfg;
	config_setting_t *setting;

	config_init(&cfg);

	if(!config_read_file(&cfg, configFile))
	{
		config_destroy(&cfg);
		return;
	}

	setting = config_lookup(&cfg, "params");

	if(setting == NULL)
	{
		config_destroy(&cfg);
		return;
	}
	const char *editor;
	config_setting_lookup_string(setting, "TextEditor", &editor);
	textEditor = strdup(editor);
	// Read and parse colors. This can be done by someone else. I have achieved everything I need with this app.
	config_destroy(&cfg);
}
