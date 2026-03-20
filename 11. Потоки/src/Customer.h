#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Stall.h"

#define COUNT_CUSTOMERS 3
#define COUNT_HUNGER 9900

struct Customer{
	int id;
	int hunger;
	pthread_t thread;
	struct Stall* stalls;
};

void* Eat(void* args);
void InitCustomers(struct Customer* customers, struct Stall* stalls);
