#include "TUIFunc.h"

void InitNcursesStuff(){
	initscr();
	refresh();
}

void InitWorkspace(struct Workspace* workspace, int h, int w, int y, int x){
	InitNcursesStuff();	
	
	workspace->win_msg = newwin(h, w, y, x);
	box(workspace->win_msg, 0, 0);	
	workspace->win_text = newwin(3, w, (y + h + 2), x);
	box(workspace->win_text, 0, 0);

	RefreshAllWin(workspace);
}

void PrintMsgs(WINDOW* win, struct List* list_msg){
	wclear(win);
	int i = 1;
	struct Node* node = list_msg->start;
	while(node != NULL){
		mvwprintw(win, i, 1, "%s\n", node->msg);
		i++;
		node = node->next;
	}
	box(win, 0, 0);
	wrefresh(win);
}

void RefreshAllWin(struct Workspace* workspace){
	wrefresh(workspace->win_msg);
	wrefresh(workspace->win_text);
}

void StartWorkspace(struct Client* client, struct Workspace* workspace){
	while (1) {
		char msg[SIZE_MSG_SEND];
		mvwgetstr(workspace->win_text, 1, 1, msg);
		werase(workspace->win_text);
		box(workspace->win_text, 0, 0);
		wrefresh(workspace->win_text);

		int recv_send = SendMsg(client, msg);
		if (recv_send) {
			int err = errno;
			printw("StartWorkspace %d: не удалось отправить сообщение", err);
			refresh();
		}

		if ( IsItExitMsg(msg) ) {
			return;
		}

		int recv_get = GetMsg(client);
		PrintMsgs(workspace->win_msg, client->list_msg);
	}
}
