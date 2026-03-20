#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Stall.h"

#define COUNT_LOAD 200

struct Loader{
	int load;
	pthread_t thread;
	struct Stall* stalls;
};

void InitLoader(struct Loader* loader, struct Stall* stalls);
void* LoadUp(void* args);
