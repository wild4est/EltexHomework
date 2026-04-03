#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char* argv[]){
	if (argc != 3){
		return;
	}

	int pid = (int)strtol(argv[1], NULL, 10);
	int sig = (int)strtol(argv[2], NULL, 10);
	if (kill(pid, sig) != -1){
		printf("Процессу[PID = %d] послан сигнал %d\n", pid, sig);
	}else{
		printf("Сигнал не удалось послать\n");
	}

}
