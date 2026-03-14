#include "Tasks.h"

int task1(){
	struct NodeProcess* np = InitNodeProcess();
	np->Left = InitNodeProcess();
	
	StartFork(np);
	FreeNodeProcess(np);
	return 0;
}


int task2(){
	/*        (s)
	 *       /   \
	 *     (1)   (2)
	 *    /  \    |
	 *  (3)  (4) (5)
	 */

	struct NodeProcess* np = InitNodeProcess();
	np->Left = InitNodeProcess();
	np->Right = InitNodeProcess();	
	np->Left->Left = InitNodeProcess();
	np->Left->Right = InitNodeProcess();
	np->Right->Left = InitNodeProcess();
	
	StartFork(np);
	FreeNodeProcess(np);

	return 0;
}

void DeleteEnter(char* str){
	for (int i=0; i<SIZE_COMBUFF; i++) {
		if (str[i] == '\n') {
			str[i] = '\0';
			return;
		}
	}
}

void task3(){
	char combuff[SIZE_COMBUFF];
	
	while (1) {
		printf(">> ");
		fgets(combuff, SIZE_COMBUFF, stdin);
		DeleteEnter(combuff);
	
		char* token = strtok(combuff, " ");
		char** words = malloc(sizeof(char*) * 10);
		int i=0;
		while (token != NULL) {
			words[i] = malloc(sizeof(char) * SIZE_COMBUFF);
			strcpy(words[i],  token);
			i++;
			token = strtok(NULL, " ");
		}
		words[i] = NULL;
		
		if (words[0] == NULL) {
			continue;
		}


		if (strcmp(words[0], "exit")==0) {
			return;
		}

		pid_t pid = fork();
		if (pid < 0) {
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			execvp(words[0], words);
			exit(EXIT_FAILURE);
		}else{
			int status;
			int return_pid = wait(&status);
			printf("\n");
		}
	}
}
