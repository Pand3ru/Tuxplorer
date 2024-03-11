#include<unistd.h>
#include<stdlib.h>

#include "../include/controls.h"
#include "../include/globals.h"
#include "../include/draw.h"
#include "../include/dirFunctions.h"

void ctrlFolderView()
{
	int amt, sel = 0, start = 0, itemAmount = LINES-4-4; /* Calculates number of max. Printed items */

	int boolean = 1;

	char path[1024];

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
					snprintf(command, sizeof(command), "nvim %s", dir[sel]->name);
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
			default:
				continue;
		}
		wclear(leftPanelWindow);
		mvwprintw(leftPanelWindow, 1, 1, "Selected Item: %d", sel);
		mvwprintw(leftPanelWindow, 2, 1, "Offset: %d", start);
		mvwprintw(leftPanelWindow, 3, 1, "Total items: %d", amt);
		mvwprintw(leftPanelWindow, 4, 1, "Total page items: %d", itemAmount);
		printFolderMenu(rightPanelWindow,  sel%amt, start);
	}
	endwin();
}
