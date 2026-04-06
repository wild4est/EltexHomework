#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int IsNum(char* str){
	int i = 0;
	while(str[i] != '\0'){
		if ( str[i] < '0' || '9' < str[i]){
			return 0;
		}
		i++;
	}
	return 1;
}


void main(int argc, char* argv[]){
	if (argc != 3){
		return;
	}
	if (!IsNum(argv[2])) {
		printf("Некоректный сигнал\n");
		return;
	}

	pid_t pid = (pid_t)strtol(argv[1], NULL, 10);
	int sig = (int)strtol(argv[2], NULL, 10);
	if (pid == 0) {
		printf("Некоректный pid\n");
		return;
	}
	
	if (kill(pid, sig) != -1){
		printf("Процессу[PID = %d] послан сигнал %d\n", pid, sig);
	}else{
		printf("Сигнал не удалось послать\n");
	}

}
