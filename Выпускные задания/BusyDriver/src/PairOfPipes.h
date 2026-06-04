#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE_BUFF 10

struct PairOfPipes {
	int pipe_from_child[2];
	int pipe_from_parent[2];
};

int InitPairOfPipes(struct PairOfPipes* pair_of_pipes);
int ClosePairForChild(struct PairOfPipes* pair_of_pipes);
int ClosePairForParent(struct PairOfPipes* pair_of_pipes);
int GetMsg(int pipefd[2], char* msg);
int SendMsg(int pipefd[2], char* msg);
