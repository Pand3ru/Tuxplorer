#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include "../include/controls.h"
#include "../include/globals.h"
#include "../include/draw.h"
#include "../include/dirFunctions.h"

void ctrlFolderView()
{
	int amt, sel = 0, start = 0, itemAmount = LINES-4-4; /* Calculates number of max. Printed items */

	int boolean = 1;

	char path[1024], input[25];

	if(getcwd(path, sizeof(path)) == NULL)
	{
		printf("Error reading Path");
	}

	dir = dirGetContent(path, &amt);

	printFolderMenu(rightPanelWindow, 0, start);

	while(boolean)
	{
		char c = wgetch(rightPanelWindow);

		switch(c)
		{
			case 'j':
				if(sel >= itemAmount && !((itemAmount + start) > sel))
				{
					start++;
				} 
				if(sel == amt - 1)
				{
					start = 0;
				}
				sel = (sel + 1) % amt;
				break;
			case 'k':
				if(sel <= start && sel != 0)
				{
					start--;
				}
				else if(sel == 0 && amt > itemAmount)
				{
					start = amt-itemAmount-1;
				}
				sel = (sel - 1 + amt) % amt;
				break;
			case '\n':
				if(dir[sel]->s == F_FALSE)
				{
					char command[256]; 
					snprintf(command, sizeof(command), "%s %s", textEditor, dir[sel]->name);
					system(command);
				}
				wclear(footerWindow);
				chdir(dir[sel]->name);
				boolean = 0;
				break;
			case 'i': /* toggle hidden Content */
				ignoreInvis = !ignoreInvis;
				return;
			case 127: /* Backspace */
				chdir("..");
				return;
			case 'q':
				terminateWindow(rightPanelWindow);
				terminateWindow(leftPanelWindow);
				terminateWindow(footerWindow);
				endwin();
				exit(0);
			case 'r':
				/* refactor to: getInput and return input. Maybe with pointer parameter. Would probably always be: Do that. scan. u sure? return string*/
				promtMakeInputReady();

				mvwprintw(footerWindow, 1, 1, "Rename %s to: ", dir[sel]->name);

				box(footerWindow, 0, 0);
				wrefresh(footerWindow);

				mvwscanw(footerWindow, 1, 13 + strlen(dir[sel]->name), "%s", input);

				wrefresh(footerWindow);

				if(promtConfirmation() && dir[sel]->s == F_FALSE)
				{
					rename(dir[sel]->name, input);	
				} 
				else
				{
					break;
				}
				return;
			case 'd':
				promtMakeInputReady();
				
				if(promtConfirmation() && dir[sel]->s == F_FALSE)
				{
					remove(dir[sel]->name);
					return;
				} 
				break;

			default:
				continue;
		}
		wclear(leftPanelWindow);

		mvwprintw(leftPanelWindow, 1, 1, "Selected Item: %d", sel);
		mvwprintw(leftPanelWindow, 2, 1, "Offset: %d", start);
		mvwprintw(leftPanelWindow, 3, 1, "Total items: %d", amt);
		mvwprintw(leftPanelWindow, 4, 1, "Total page items: %d", itemAmount);
		mvwprintw(leftPanelWindow, 5, 1, "Input: %s at %p", input, &input);
		mvwprintw(leftPanelWindow, 6, 1, "Lenght: %lu", 12 + strlen(dir[sel]->name));

		printFolderMenu(rightPanelWindow,  sel%amt, start);
		drawTopbars();
	}
	endwin();
}

int promtConfirmation()
{
	promtMakeInputReady();

	mvwprintw(footerWindow, 1, 1, "Are you sure? [y/N] ");

	box(footerWindow, 0, 0);
	wrefresh(footerWindow);

	char sure = mvwgetch(footerWindow, 1, 20);

	noecho();
	curs_set(0);

	if(toupper(sure) == 'Y')
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}

void promtMakeInputReady()
{
	curs_set(1);
	echo();
	flushinp();
	wclear(footerWindow);
}
