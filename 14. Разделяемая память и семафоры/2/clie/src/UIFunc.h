#pragma once

#include <malloc.h>
#include <ncurses.h>
#include <errno.h>
#include <stdio.h>

#include "ClientFunc.h"

#define START_X 1
#define START_Y 1

#define WIN_MSGS_H 12
#define WIN_MSGS_W 40

#define WIN_TEXT_H 10
//#define WIN_TEXT_W 10

#define WIN_CLIENTS_H 12
#define WIN_CLIENTS_W 40

struct Workspace{
	WINDOW* win_msgs;
	WINDOW* subwin_msgs;

	WINDOW* win_text;
	WINDOW* subwin_text;

	WINDOW* win_clients;
	WINDOW* subwin_clients;

};

//struct Workspace* workspace;

void InitNcursesStuff();
void InitWorkspace();
void RefreshAllWin();
void PrintList(WINDOW* win, struct List* list);
void* MessagingListener(void* args);
void* BroadcastListener(void* args);
