#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Customer.h"
#include "Loader.h"

//extern int AllCustomersIsFull;

void StartMarket(struct Customer* customers, struct Loader* loader);
void EndMarket(struct Customer* customers, struct Loader* loader);
