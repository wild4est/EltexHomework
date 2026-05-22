#include "UIFunc.h"

struct Workspace workspace;

void InitNcursesStuff(){
	initscr();
	refresh();
}

void InitWorkspace(){
	workspace.win_msgs = newwin(WIN_MSGS_H, WIN_MSGS_W, START_Y, START_X);
	box(workspace.win_msgs, 0, 0);

	workspace.win_clients = newwin(WIN_CLIENTS_H, WIN_CLIENTS_W, START_Y, WIN_MSGS_W+2);
	box(workspace.win_clients, 0, 0);

	workspace.win_text = newwin(WIN_TEXT_H, (WIN_MSGS_W + WIN_CLIENTS_W), WIN_MSGS_H+2, START_X);

	RefreshAllWin();

	printw("Для выхода нажмите CTR + C");
	mvprintw(WIN_CLIENTS_H+1, START_X, "_________________________________________________________________________________");
	mvprintw(WIN_CLIENTS_H+WIN_TEXT_H+3, START_X, "_________________________________________________________________________________");

	refresh();
}

void PrintList(WINDOW* win, struct List* list){
	wclear(win);
	int i = 1;
	struct Node* node = list->start;
	while(node != NULL){
		mvwprintw(win, i, 1, "%s\n", node->data);
		i++;
		node = node->next;
	}
	box(win, 0, 0);
	wrefresh(win);
}

void RefreshAllWin(){
	wrefresh(workspace.win_msgs);
	wrefresh(workspace.win_clients);
	wrefresh(workspace.win_text);
}

void* MessagingListener(void* args){
	struct Client* client = (struct Client*)args;
	while(1){
		char msg[SIZE_BUFF_MSG];
		mvwgetstr(workspace.win_text, 1, 1, msg);
		werase(workspace.win_text);
		wrefresh(workspace.win_text);

		int recv = SendMsgToServer(client->mqd_messaging, client->name, msg);
		if (recv) {
			int err = errno;
			printw("MessagingListener %d: не удалось отправить сообщение", err);
			refresh();
		}
	}	
}

void* BroadcastListener(void* args){
	struct Client* client = (struct Client*)args;
	while(1){
		int recv = GetMsgFromServer(client->mqd_personal, &(client->list_msgs), &(client->list_clients));
		if (recv) {
			int err = errno;
			printw("BroadcastListener %d: не удалось отправить сообщение", err);
			refresh();
		}
		PrintList(workspace.win_msgs, client->list_msgs);
		PrintList(workspace.win_clients, client->list_clients);
	}
}

