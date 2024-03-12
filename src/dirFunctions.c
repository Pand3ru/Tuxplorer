#include<dirent.h>
#include<stdlib.h>
#include<string.h>

#include "../include/dirFunctions.h"
#include "../include/globals.h"

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

struct dirContent** dirGetPinned(int *size)
{
	homeDir = getenv("HOME");	
	if(homeDir == NULL)
	{
		return NULL;
	}
	
	int bufSize = 1024, i = 0;
	char *filePath = malloc(bufSize);

	int needed = snprintf(filePath, bufSize, "%s/.config/tuxplorer/config.ini", homeDir);

	char *tmpFilePath = realloc(filePath, needed + 1);

	if(tmpFilePath != NULL)
	{
		filePath = tmpFilePath;
	}

	/* Config Path aquired */

	FILE* fptr = fopen(filePath, "r");

	if(fptr == NULL)
	{
		free(filePath);
		return NULL;
	}
	
	char *currentLine;

	struct dirContent **content = NULL;

	while((currentLine = readLine(fptr)) != NULL)
	{
		content = realloc(content, (i + 1) * sizeof(struct dirContent*));
		if(content == NULL)
		{
			return NULL;
		}

		content[i] = malloc(sizeof(struct dirContent));
		if(content[i] == NULL)
		{
			return NULL;
		}
		char* token = strtok(currentLine, " ");
		content[i]->name = strdup(token);

		token = strtok(NULL, " ");
		content[i]->path = strdup(token);
		content[i]->s = F_TRUE;

		free(currentLine);
		i++;
	}
	free(filePath);
	*size = i;
	return content;
}

char *readLine(FILE *file) 
{
    if (file == NULL)
    {
        return NULL;
    }

    int maximumLineLength = 128;
    char *lineBuffer = malloc(maximumLineLength);
    if (lineBuffer == NULL)
    {
        return NULL;
    }

    int count = 0;
    char ch = getc(file);
    
    if(ch == EOF)
    {
	    return NULL;
    }

    while ((ch != '\n') && (ch != EOF)) 
    {
        if (count == maximumLineLength - 1) 
	{
            maximumLineLength += 128;
            char *tempBuffer = realloc(lineBuffer, maximumLineLength);
            if (tempBuffer == NULL) 
	    {
                free(lineBuffer);
                return NULL;
            }
            lineBuffer = tempBuffer;
        }
        lineBuffer[count++] = ch;
        ch = getc(file);
    }

    lineBuffer[count] = '\0';
    return lineBuffer; 
}
