#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <malloc.h>
#include <errno.h>

#include "List.h"
#include "ClientFunc.h"

struct Workspace{
	WINDOW* win_msg;
	WINDOW* win_text;
};

void InitNcursesStuff();
void InitWorkspace(struct Workspace* workspace, int h, int w, int y, int x);
void PrintMsgs(WINDOW* win, struct List* list_msg);
void RefreshAllWin(struct Workspace* workspace);
void StartWorkspace(struct Client* client, struct Workspace* workspace);
