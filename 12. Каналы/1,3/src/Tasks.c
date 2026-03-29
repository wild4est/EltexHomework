#include "Tasks.h"

int task1(){
	char msg[] = "Hi!";
	int pipefd[2];
	pid_t pid;

	if (pipe(pipefd) == -1) {
		return 1;
	}

	pid = fork();
	if (pid == -1) {
		return 2;
	}
	if (pid == 0) { //Дочерний
		if (close(pipefd[1]) == -1) {
			return 3;
		}
		char buff[SIZE_BUFF];
		if (read(pipefd[0], buff, sizeof(buff)) == -1){
			return 5;
		}
		printf("Дочерний процесс прочитал сообщение: %s\n", buff);
		exit(EXIT_SUCCESS);

	}else{ //Родительский
		if (close(pipefd[0]) == -1) {
			return 3;
		}
		if (write(pipefd[1], msg, strlen(msg)) != strlen(msg)) {
			return 4;
		}
		printf("Родительский процесс отправил сообщение: %s\n", msg);
		if (close(pipefd[1]) == -1) {
			return 3;
		}

		int status;
		wait(&status);
		printf("Дочерний процесс завершил работу\n");
	}

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

char** Split(char* ch, char* combuff, int* n){
	char* token = strtok(combuff, ch);
	char** substrs = malloc(sizeof(char*) * SIZE_COMMANDS);
	while (token != NULL) {
		substrs[*n] = malloc(sizeof(char) * SIZE_COMBUFF);
		strcpy(substrs[*n], token);
		(*n)++;
		token = strtok(NULL, ch);
	}
	return substrs;
}

void FreeStrArr(char** arr){
	for (int i=0; i<SIZE_COMMANDS; i++) {
		free(arr[i]);
	}
	free(arr);
}

void task3(){
	char combuff[SIZE_COMBUFF];

	while (1) {
		printf(">> ");
		fgets(combuff, SIZE_COMBUFF, stdin);
		DeleteEnter(combuff);

		if (strcmp(combuff, "exit")==0) {
			return;
		}

		int command_count = 0;
		char** commands = Split("|", combuff, &command_count);
		int tmp_pipefd;
		for (int i=0; i<command_count; i++) {
			int words_count=0;
			char** words = Split(" ", commands[i], &words_count);
			words[words_count] = NULL;

			if (words[0] == NULL) {
				continue;
			}

			int pipefd[2];
			if (i < (command_count - 1)) {
				pipe(pipefd);
			}

			pid_t pid = fork();
			if (pid < 0) {
				exit(EXIT_FAILURE);
			}
			if (pid == 0) {
				if (i < (command_count - 1)) {
					dup2(pipefd[1], 1);
					close(pipefd[1]);
				}
				if (i > 0) {
					dup2(tmp_pipefd, 0);
					close(tmp_pipefd);
				}
				execvp(words[0], words);
				exit(EXIT_FAILURE);
			}
			if (i < (command_count - 1)) {
				close(pipefd[1]);
			}
			if (i > 0) {
				close(tmp_pipefd);
			}
			tmp_pipefd = pipefd[0];
			FreeStrArr(words);
		}

		for (int i=0; i<command_count; i++) {
			int status;
			int return_pid = wait(&status);
		}
		printf("\n");
		FreeStrArr(commands);
	}
}

