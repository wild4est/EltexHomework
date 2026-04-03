#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void main(){
        printf("PID = %d\n", getpid());
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
        sigprocmask(SIG_BLOCK, &set, NULL);
        while(1){}
}

