#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void main(){
        printf("PID = %d\n", getpid());
       
	sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

	int sig;
	while(1){
		sigwait(&set, &sig);
		printf("Получен сигнал %d\n", sig);
	}
}

