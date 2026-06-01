#include "TUIFunc.h"

void InitNcursesStuff(){
	initscr();
	refresh();
}

void InitWorkspace(struct Workspace* workspace, int h, int w, int y, int x){
	InitNcursesStuff();	
	
	printw("Для выхода нажмите CTR+C, либо напишите сообщение %s\n", EXIT_CODE);
	workspace->win_msg = newwin(h, (w + 6), (y+1), (x+1));
	box(workspace->win_msg, 0, 0);	
	workspace->win_text = newwin(3, w, (y + h + 3), (x + 4));
	box(workspace->win_text, 0, 0);

	RefreshAllWin(workspace);

	refresh();
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

int StartWorkspace(struct Client* client, struct Workspace* workspace){
	while (1) {
		char msg[SIZE_MSG_SEND];
		mvwgetnstr(workspace->win_text, 1, 1, msg, 48);
		werase(workspace->win_text);
		box(workspace->win_text, 0, 0);
		wrefresh(workspace->win_text);

		int recv_send = SendMsg(client, msg);
		if (recv_send == -1) {
			int err = errno;
			printw("StartWorkspace %d: не удалось отправить сообщение", err);
			refresh();
		}

		int recv_get = GetMsg(client);
		if (recv_get == -1) {
			int err = errno;
			printw("StartWorkspace %d: не удалось принять сообщение", err);
			refresh();
		}else if (recv_get == 1) {
			//printw("Сервер всё");
			//refresh();
			endwin();
			return -1;
		}

		PrintMsgs(workspace->win_msg, client->list_msg);
	}
	return 0;
}
