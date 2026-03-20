#ifndef STALL_H
#define STALL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_STALLS 5
#define COUNT_PRODUCT 900

struct Stall{
	int id;
	int count_product;
	pthread_mutex_t mutex;
};

void InitStalls(struct Stall* stalls);
#endif /* STALL_H */
