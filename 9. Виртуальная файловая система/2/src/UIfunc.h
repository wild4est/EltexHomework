#include <ncurses.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>


#define PATH_SIZE 256

enum ColorPairs{
	DIR_CP=2,
	LNK_CP,
	REG_CP,
	UNKNOWN_CP,

	CH_DIR_CP,
	CH_LNK_CP,
	CH_REG_CP,
	CH_UNKNOWN_CP
};

struct Workspace{
	int x, y;
	int width, height;
	int size_namelist;
	int chosen_dir;
	char path[PATH_SIZE];
	WINDOW* win;
	WINDOW* subwin;
	struct dirent **namelist;
};

struct Workspace* InitWorkspace(int ws_x, int ws_y, int ws_w, int ws_h, char* ws_path);
void PrintWorkspace(struct Workspace* workspace, int choose);
void MainScreen();
int GetColorPair(unsigned char dir_type);
void FreeNamelist(struct dirent** namelist, int n);
void FreeWorkspace(struct Workspace* workspace);
void CutLastDir(char* path);
void InitColorPairs();
void RecolorDir(struct Workspace* workspace, int dir1, int dir2);
