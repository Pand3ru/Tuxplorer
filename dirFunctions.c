#include<dirent.h>
#include<stdlib.h>
#include<string.h>

#include "dirFunctions.h"

struct dirContent **dirGetContent(const char *path)
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

	if(d != NULL)
	{
		while((entries = readdir(d)) != NULL)
		{
			if(i >= size)
			{
				size *= 2;
				struct dirContent **newContent = realloc(content, size * sizeof(struct dirContent*));
				if(newContent == NULL)
				{
					closedir(d);
					return NULL;
				}
				content = newContent;
			}

			content[i] = malloc(sizeof(struct dirContent));
			content[i]->name = strdup(entries->d_name);
			content[i]->s = (entries->d_type == DT_DIR ? F_TRUE : F_FALSE);

			if(content[i] == NULL)
			{
				closedir(d);
				return NULL;
			}
			i++;
		}
		content[i] = NULL;
	} else {
		free(content);
		return NULL;
	}
	closedir(d);

	struct dirContent **final = realloc(content, (i+1) * sizeof(struct dirContent*));
	if(final == NULL)
	{
		return content;
	}
	content = final;
	content[i] = NULL;

	return content;
}
