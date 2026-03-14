#include "NodeProcess.h"

struct NodeProcess* InitNodeProcess(){
	struct NodeProcess* np = malloc(sizeof(struct NodeProcess));
	np->Left = NULL;
	np->Right = NULL;
	return np;
}

void FreeNodeProcess(struct NodeProcess* np){
	if (np->Left != NULL) {
		FreeNodeProcess(np->Left);
	}

	if (np->Right != NULL) {
		FreeNodeProcess(np->Right);
	}
	
	free(np);
}


void StartFork(struct NodeProcess* np){
	np->pid = getpid();
	printf("[ pid = %d | ppid = %d ]\n", getpid(), getppid());

	if (np->Left != NULL) {
		pid_t child_pid = fork();
		if (child_pid < 0) {
			exit(EXIT_FAILURE);
		}

		if (child_pid == 0) { //Дочерний
			StartFork(np->Left);
			exit(EXIT_SUCCESS);
		}else {
			int status;
			int return_pid = wait(&status);
			printf("Дочерний процесс %d заверил работу со статусом %d\n", return_pid, status);
		}
	}

	if (np->Right != NULL) {
		pid_t child_pid = fork();
		if (child_pid < 0) {
			exit(EXIT_FAILURE);
		}

		if (child_pid == 0) { //Дочерний
			StartFork(np->Right);
			exit(EXIT_SUCCESS);
		}else {
			int status;
			int return_pid = wait(&status);
			printf("Дочерний процесс %d заверил работу со статусом %d\n", return_pid, status);
		}
	}

	return;
	//exit(EXIT_SUCCESS);
}

