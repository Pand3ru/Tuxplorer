#include<dirent.h>
#include<ncurses.h>

#include "draw.h"

int main(int argc, char *argv[])
{
	drawLayout(".");

	endwin();
	return 0;
}

