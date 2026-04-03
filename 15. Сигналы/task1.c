#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void PrintMsgHandler(){
	printf("Получен сигнал SIGUSR1\n");
	exit(EXIT_SUCCESS);
}

void main(){
	printf("PID = %d\n", getpid());
	struct sigaction act;
	act.sa_handler = PrintMsgHandler;
	sigaction(SIGUSR1, &act, NULL);
	while(1){}
}
