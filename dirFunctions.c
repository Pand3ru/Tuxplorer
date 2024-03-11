#include<dirent.h>
#include<stdlib.h>
#include<string.h>

#include "dirFunctions.h"
#include "globals.h"

/* Function to return a **dirContent so I can get Dir Content quickly. Probably slow af */
struct dirContent **dirGetContent(const char *path, int *amt)
{
	DIR *d;
	struct dirent *entries;
	int i = 0, size = 10;

	struct dirContent **content = malloc(size * sizeof(struct dirContent*));
	if(content == NULL)
	{
		return NULL;
	}

	d = opendir(path);

	if(d == NULL)
	{
		free(content);
		return NULL;
	}

	char firstLetter;
	
	while((entries = readdir(d)) != NULL)
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
			closedir(d);   
			return NULL;
			}
			content = newContent;
		}

		firstLetter = entries->d_name[0];

		if(!ignoreInvis && firstLetter == '.')
		{
			continue;
		}

		content[i] = malloc(sizeof(struct dirContent));
		content[i]->name = strdup(entries->d_name);

		if(content[i]->name == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(content[j]->name); 
				free(content[j]);      
			}
			free(content); 
			closedir(d);   
			return NULL;
		}
		
		content[i]->s = (entries->d_type == DT_DIR ? F_TRUE : F_FALSE);

		i++;
	}
	content[i] = NULL;
	closedir(d);

	struct dirContent **final = realloc(content, (i+1) * sizeof(struct dirContent*));
	if(final == NULL)
	{
		return content;
	}
	content = final;
	content[i] = NULL;
	*amt = i;

	return content;
}
