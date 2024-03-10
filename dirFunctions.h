#ifndef DIRFUNCTIONS_H
#define DIRFUNCTIONS_H

enum StatusFolder 
{
	F_TRUE,
	F_FALSE
};

struct dirContent
{
	enum StatusFolder s;
	char *name;
};

struct dirContent **dirGetContent(const char *path, int *size);

#endif
