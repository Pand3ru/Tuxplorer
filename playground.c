#include<ncurses.h>

int main()
{
	initscr();
	printw("Hi mom");
	refresh();
	getch();
	endwin();
	return 0;
}

