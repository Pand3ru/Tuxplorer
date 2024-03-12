#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include "../include/controls.h"
#include "../include/globals.h"
#include "../include/draw.h"
#include "../include/dirFunctions.h"

void windowSelector()
{
	char cwd[1024];
	int amt, amt2;

	pins = dirGetPinned(&amt);
	dir = dirGetContent(getcwd(cwd, sizeof(cwd)), &amt2);

	printFolderMenu(leftPanelWindow, -1, 0, 2, pins);
	printFolderMenu(rightPanelWindow, -1, 0, 3, dir);
	while(1)
	{
		char ch = getch();
		switch(ch)
		{
			case 'j':
			case 'k':
				selectedWindow = !selectedWindow;
				break;
			case 'q':
				terminateWindow(rightPanelWindow);
				terminateWindow(leftPanelWindow);
				terminateWindow(footerWindow);
				endwin();
				exit(0);
				return;
			case '\n':
				isInWindowSelection = 0;
				if(selectedWindow == 1)
				{
					ctrlFolderView();
					return;
				} else {
					ctrlPinView();
					return;
				}
			default:
				continue;
		}
		printFolderMenu(leftPanelWindow, -1, 0, 2, pins);
		printFolderMenu(rightPanelWindow, -1, 0, 3, dir);
	}
}
void ctrlPinView()
{
	int amt, sel = 0, start = 0, itemAmount = LINES-4-4; /* Calculates number of max. Printed items */

	int boolean = 1;

	dirGetPinned(&amt);

	printFolderMenu(leftPanelWindow, 0, start, 2, pins);
	printFolderMenu(rightPanelWindow, -1, 0, 3, dir);

	while(boolean)
	{
		char c = wgetch(leftPanelWindow);

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
				wclear(footerWindow);
				chdir(pins[sel]->path);
				boolean = 0;
				selectedWindow = 1;
				// Find a way to switch back. Maybe change void? if return = 1 start ctrlFolderView
				return;
			case 'q':
				terminateWindow(rightPanelWindow);
				terminateWindow(leftPanelWindow);
				terminateWindow(footerWindow);
				endwin();
				exit(0);
			case 27: /* Esc */
				isInWindowSelection = 1;
				return;
			default:
				continue;
		}
		printFolderMenu(leftPanelWindow,  sel%amt, start, 2, pins);
		drawTopbars();
	}
	endwin();

}

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

	printFolderMenu(rightPanelWindow, 0, start, 3, dir);
	printFolderMenu(leftPanelWindow, -1, 0, 2, pins);

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
				promtMakeInputReady();

				promtGetInput(input, "Rename to:");

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
			case 'c':
				promtMakeInputReady();

				promtGetInput(input, "Create File:");

				FILE* f = fopen(input, "a");
				fclose(f);
				return;
			case 27: /* Esc */
				isInWindowSelection = 1;
				return;
			default:
				continue;
		}
		printFolderMenu(rightPanelWindow,  sel%amt, start, 3, dir);
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
void promtGetInput(char *input, char *text)
{
	promtMakeInputReady();

	mvwprintw(footerWindow, 1, 1, "%s", text);

	box(footerWindow, 0, 0);
	wrefresh(footerWindow);

	mvwscanw(footerWindow, 1, strlen(text) + 2, "%s", input);

	wrefresh(footerWindow);
}
