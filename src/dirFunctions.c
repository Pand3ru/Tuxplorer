#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include<libconfig.h>

#include "../include/dirFunctions.h"
#include "../include/globals.h"

/* Function to return a **dirContent so I can get Dir Content quickly. Probably slow af */
struct dirContent **dirGetContent(const char *path)
{
	struct dirent **entries;
	int i = 0, size = 10;

	struct dirContent **content = malloc(size * sizeof(struct dirContent*));
	if(content == NULL)
	{
		return NULL;
	}

	char firstLetter;
	int n = scandir(path, &entries, NULL, alphasort);	

	if(n == -1)
	{
		return NULL;
	}

	for(int index = 0; index < n; index++)
	{
		if(i >= size)
		{
			size *= 2;
			struct dirContent **newContent = realloc(content, size * sizeof(struct dirContent*));
			if(newContent == NULL)
			{
				for (int j = 0; j < i; j++)
				{
					free(content[j]->name); 
					free(content[j]);      
				}
			free(content); 
			return NULL;
			}
			content = newContent;
		}

		firstLetter = entries[index]->d_name[0];

		if(!ignoreInvis && firstLetter == '.')
		{
			continue;
		}

		content[i] = malloc(sizeof(struct dirContent));
		content[i]->name = strdup(entries[index]->d_name);

		if(content[i]->name == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(content[j]->name); 
				free(content[j]);      
			}
			free(content); 
			return NULL;
		}
		
		content[i]->s = (entries[index]->d_type == DT_DIR ? F_TRUE : F_FALSE);

		i++;
	}

	content[i] = NULL;

	struct dirContent **final = realloc(content, (i+1) * sizeof(struct dirContent*));
	if(final == NULL)
	{
		return content;
	}
	content = final;
	content[i] = NULL;
	amount_folder = i;

	return content;
}
void freePins()
{
	for(int i = 0; i < amount_pins; i++)
	{
		free(pins[i]->name);
		free(pins[i]->path);
		free(pins[i]);
	}
	free(pins);
}
void dirGetPinned()
{
	freePins();
	int i = 0;
	struct dirContent **pinnedDir = NULL;

	config_t cfg;
	config_setting_t *setting, *currentElement;

	config_init(&cfg);

	if(!config_read_file(&cfg, configFile))
	{
		printf("\n%s:%d - %s | %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg), configFile);
		config_destroy(&cfg);
		return;
	}

	setting = config_lookup(&cfg, "pins");
	if(setting == NULL)
	{
		config_destroy(&cfg);
		return;
	}

	int c = config_setting_length(setting);

	for(;i < c; i++)
	{
		const char *name, *path;
		pinnedDir = realloc(pinnedDir, (i+1) * sizeof(struct dirContent*));
		currentElement = config_setting_get_elem(setting, i);

		name = config_setting_name(currentElement);
		config_setting_lookup_string(setting, name, &path);

		pinnedDir[i] = malloc(sizeof(struct dirContent));
		if(pinnedDir[i] == NULL)
		{
			for(int j = 0; j < i; j++)
			{
				free(pinnedDir[j]->name);
				free(pinnedDir[j]->path); 
				free(pinnedDir[j]);
			}
			free(pinnedDir);
			return;
		}
		pinnedDir[i]->name = strdup(name);
		pinnedDir[i]->path = strdup(path);
		pinnedDir[i]->s = F_TRUE;
	}
	pinnedDir = realloc(pinnedDir, (i+1) * sizeof(struct dirContent*));
	pinnedDir[i] = NULL;

	config_destroy(&cfg);
	amount_pins = c;

	pins = pinnedDir;
}
