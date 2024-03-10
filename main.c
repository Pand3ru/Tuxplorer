#include<dirent.h>
#include<ncurses.h>
#include<stdio.h>

#include "draw.h"
#include "dirFunctions.h"

int main(int argc, char *argv[])
{
	drawLayout(".");

	endwin();
	return 0;
}

