#include "PairOfPipes.h"

int InitPairOfPipes(struct PairOfPipes* pair_of_pipes) {
	if (pipe(pair_of_pipes->pipe_from_child) == -1) {
		return -1;
	}
	if (pipe(pair_of_pipes->pipe_from_parent) == -1) {
		return -1;
	}
	return 0;
}

int ClosePairForChild(struct PairOfPipes* pair_of_pipes) {
	if (close(pair_of_pipes->pipe_from_child[0]) == -1) {
		return -1;
	}
	if (close(pair_of_pipes->pipe_from_parent[1]) == -1) {
		return -1;
	}
	return 0;
}

int ClosePairForParent(struct PairOfPipes* pair_of_pipes) {
	if (close(pair_of_pipes->pipe_from_child[1]) == -1) {
		return -1;
	}
	if (close(pair_of_pipes->pipe_from_parent[0]) == -1) {
		return -1;
	}
	return 0;
}

int GetMsg(int pipefd[2], char* msg) {
	char buff[SIZE_BUFF];
	if (read(pipefd[0], buff, sizeof(buff)) == -1) {
		int err = errno;
		if (err != EINTR) {
			printf("Error: GetMsg = %d\n", err);
			return -1;
		}
		return 1;
	}
	strcpy(msg, buff);

	return 0;
}

int SendMsg(int pipefd[2], char* msg) {
	if (write(pipefd[1], msg, (strlen(msg) + 1)) == -1) {
		int err = errno;
		printf("Error: SendMsg = %d\n", err);
		return -1;
	}
	return 0;
}

