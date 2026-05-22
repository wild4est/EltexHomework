#include "UIfunc.h"

struct Workspace* InitWorkspace(int ws_x, int ws_y, int ws_w, int ws_h, char* ws_path){
	struct Workspace* workspace = malloc(sizeof(struct Workspace));
	
	workspace->x = ws_x;
	workspace->y = ws_y;
	workspace->width = ws_w;
	workspace->height = ws_h;
	strcpy(workspace->path, ws_path);
	workspace->chosen_dir = 0;
	
	workspace->win = newwin(ws_h, ws_w, ws_y, ws_x);
	box(workspace->win, 0, 0);
	
	workspace->subwin = derwin(workspace->win, ws_h-2, ws_w-2, 1, 1);
	
	workspace->size_namelist = scandir(ws_path, &(workspace->namelist), NULL, alphasort);
	return workspace;
}

int GetColorPair(unsigned char dir_type){
	switch (dir_type){
		case DT_DIR:
			return DIR_CP;
		case DT_LNK:
			return LNK_CP;
		case DT_REG:
			return REG_CP;
		default:
			return UNKNOWN_CP;
	}
}


void PrintWorkspace(struct Workspace* workspace, int choose){
	wclear(workspace->win);

	if (choose){	
		wattron(workspace->win, COLOR_PAIR(UNKNOWN_CP));
		box(workspace->win,0,0);
		wattroff(workspace->win, COLOR_PAIR(UNKNOWN_CP));
	}else{	
		box(workspace->win,0,0);
	}

	wclear(workspace->subwin);
	mvwprintw(workspace->win, 0, 2, "%s",workspace->path);
	for (int i=0; i<workspace->size_namelist; i++) {
		int cur_color_pair = GetColorPair(workspace->namelist[i]->d_type);
		if (i == workspace->chosen_dir) {
			cur_color_pair+=4;
		}
	
		wattron(workspace->subwin, COLOR_PAIR(cur_color_pair));
		wprintw(workspace->subwin, "%s\n", workspace->namelist[i]->d_name);
		wattroff(workspace->subwin, COLOR_PAIR(cur_color_pair));

	}
	wrefresh(workspace->subwin);
	wrefresh(workspace->win);
}

void FreeNamelist(struct dirent** namelist, int n){
	for (int i=0; i<n; i++){
		free(namelist[i]);
	}
	free(namelist);
}

void FreeWorkspace(struct Workspace* workspace){
	FreeNamelist(workspace->namelist, workspace->size_namelist);
	free(workspace);
}

void CutLastDir(char* path){
	int path_size = strlen(path);
	for (int i=(path_size-1); i>0; i--) {
		if (path[i] == '/'){
			path[i] = '\0';
			return;
		}
	}
	
}

void InitColorPairs(){
	init_pair(DIR_CP, COLOR_WHITE, COLOR_BLACK);
	init_pair(LNK_CP, COLOR_GREEN, COLOR_BLACK);
	init_pair(REG_CP, COLOR_BLUE, COLOR_BLACK);
	init_pair(UNKNOWN_CP, COLOR_RED, COLOR_BLACK);

	init_pair(CH_DIR_CP, COLOR_BLACK, COLOR_WHITE);
	init_pair(CH_LNK_CP, COLOR_BLACK, COLOR_GREEN);
	init_pair(CH_REG_CP, COLOR_BLACK, COLOR_BLUE);
	init_pair(CH_UNKNOWN_CP, COLOR_BLACK, COLOR_RED);
}

void RecolorDir(struct Workspace* workspace, int dir1, int dir2){
	int cur_color_pair1 = GetColorPair(workspace->namelist[dir1]->d_type);
	int cur_color_pair2 = GetColorPair(workspace->namelist[dir2]->d_type) + 4;

	mvwchgat(workspace->subwin, dir1, 0, -1, COLOR_PAIR(cur_color_pair1),cur_color_pair1,NULL);
	mvwchgat(workspace->subwin, dir2, 0, -1, A_BOLD | COLOR_PAIR(cur_color_pair2),cur_color_pair2,NULL);
	wrefresh(workspace->subwin);

}

void MainScreen(){
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();
	InitColorPairs();	
	refresh();
	printw("\n  j - вниз, k - вверх, ENTER - выбрать, TAB - переключиться на другое окно, q - выход\n");


	char path_buf[PATH_SIZE];
	if (getcwd(path_buf, sizeof(path_buf))==NULL){
		return;
	}

	int borderX = COLS/2;

	struct Workspace* workspaces[2]; 
	workspaces[0] = InitWorkspace(1, 3, borderX-2, LINES-5, path_buf);
	workspaces[1] = InitWorkspace(borderX+1, 3, borderX, LINES-5, path_buf);

	PrintWorkspace(workspaces[0], 1);
	PrintWorkspace(workspaces[1], 0);

	int chosen_ws = 0;

	int get;
	while ((get = getch()) != 'q') {
		struct Workspace** cur_ws = &(workspaces[chosen_ws]);
		
		switch (get) {
			case 'j':
				if ((*cur_ws)->chosen_dir < ((*cur_ws)->size_namelist-1)) {
					(*cur_ws)->chosen_dir++;
					RecolorDir(*cur_ws, ((*cur_ws)->chosen_dir-1), (*cur_ws)->chosen_dir);
				}
				break;

			case 'k':
				if ((*cur_ws)->chosen_dir > 0) {
					(*cur_ws)->chosen_dir--;
					RecolorDir(*cur_ws, ((*cur_ws)->chosen_dir+1), (*cur_ws)->chosen_dir);
				}
				break;

			case '\n':
				unsigned char dir_type = (*cur_ws)->namelist[(*cur_ws)->chosen_dir]->d_type; 
				char* dir_name = (*cur_ws)->namelist[(*cur_ws)->chosen_dir]->d_name;

				if (dir_type != DT_DIR){
					continue;
				}
				if (strcmp(dir_name, ".")==0) {
					continue;
				}
				if (strcmp(dir_name, "..")==0) {
					CutLastDir((*cur_ws)->path);
				}else{
					strcat((*cur_ws)->path, "/");
					strcat((*cur_ws)->path, dir_name);
				}
				FreeNamelist((*cur_ws)->namelist, (*cur_ws)->size_namelist);
				(*cur_ws)->size_namelist = scandir((*cur_ws)->path, &((*cur_ws)->namelist), NULL, alphasort);
				(*cur_ws)->chosen_dir=0;
				PrintWorkspace((*cur_ws), 1);
				break;

			case '\t':
				chosen_ws = (chosen_ws+1)%2;
				if (chosen_ws){
					PrintWorkspace(workspaces[0], 0);
					PrintWorkspace(workspaces[1], 1);
				}else{
					PrintWorkspace(workspaces[0], 1);
					PrintWorkspace(workspaces[1], 0);
				}
		}
	}

	FreeWorkspace(workspaces[0]);
	FreeWorkspace(workspaces[1]);
	endwin();
}
