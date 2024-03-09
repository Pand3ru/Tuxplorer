#include<stdio.h>
#include<dirent.h>
#include<ncurses.h>

int main(int argc, char **argv)
{
	DIR *d;

	struct dirent *ent;

	d = opendir(".");

	initscr();

	if(d != NULL)
	{
		while((ent=readdir(d)) != NULL)
		{
			if(ent->d_type == DT_DIR)
			{
				printw("DIR: %s\n", ent->d_name);
			} else {
				printw("%s\n", ent->d_name);
			}
			refresh();

		}
		getch();
		endwin();
	}


	return 0;
}
