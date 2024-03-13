#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include "../include/controls.h"
#include "../include/globals.h"
#include "../include/draw.h"
#include "../include/dirFunctions.h"
#include "../include/config.h"

void windowSelector()
{
	char cwd[1024];

	dirGetPinned();
	dir = dirGetContent(getcwd(cwd, sizeof(cwd)));

	printFolderMenu(leftPanelWindow, -1, 0, 2, pins, amount_pins);
	printFolderMenu(rightPanelWindow, -1, 0, 3, dir, amount_folder);
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
					drawTopbars();
					ctrlFolderView();
					return;
				} else {
					drawTopbars();
					ctrlPinView();
					return;
				}
			default:
				continue;
		}
		printFolderMenu(leftPanelWindow, -1, 0, 2, pins, amount_pins);
		printFolderMenu(rightPanelWindow, -1, 0, 3, dir, amount_folder);
	}
}
void ctrlPinView()
{
	int sel = 0, start = 0, itemAmount = LINES-4-4; /* Calculates number of max. Printed items */

	int boolean = 1;

	dirGetPinned();

	printFolderMenu(leftPanelWindow, 0, start, 2, pins, amount_pins);
	printFolderMenu(rightPanelWindow, -1, 0, 3, dir, amount_folder);

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
				if(sel == amount_pins - 1)
				{
					start = 0;
				}
				sel = (sel + 1) % amount_pins;
				break;
			case 'k':
				if(sel <= start && sel != 0)
				{
					start--;
				}
				else if(sel == 0 && amount_pins > itemAmount)
				{
					start = amount_pins-itemAmount-1;
				}
				sel = (sel - 1 + amount_pins) % amount_pins;
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
			case 'r':
				configRemovePin(pins[sel]);
				return;
			default:
				continue;
		}
		printFolderMenu(leftPanelWindow,  sel%amount_pins, start, 2, pins, amount_pins);
		drawTopbars();
	}
	endwin();

}

void ctrlFolderView()
{
	int sel = 0, start = 0, itemAmount = LINES-4-4; /* Calculates number of max. Printed items */

	int boolean = 1;

	char path[1024], input[25];

	if(getcwd(path, sizeof(path)) == NULL)
	{
		printf("Error reading Path");
	}

	dir = dirGetContent(path);

	printFolderMenu(rightPanelWindow, 0, start, 3, dir, amount_folder);
	printFolderMenu(leftPanelWindow, -1, 0, 2, pins, amount_pins);

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
				if(sel == amount_folder - 1)
				{
					start = 0;
				}
				sel = (sel + 1) % amount_folder;
				break;
			case 'k':
				if(sel <= start && sel != 0)
				{
					start--;
				}
				else if(sel == 0 && amount_folder > itemAmount)
				{
					start = amount_folder-itemAmount-1;
				}
				sel = (sel - 1 + amount_folder) % amount_folder;
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
			case 'p':
				//printf("%s\n", dir[sel]->name);
				if(dir[sel]->s != F_TRUE || dir[sel]->name[0] == '.')
				{
					break;
				}
				configPinFolder(dir[sel]);
				dirGetPinned();
				return;
			default:
				continue;
		}
		printFolderMenu(rightPanelWindow,  sel%amount_folder, start, 3, dir, amount_folder);
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
