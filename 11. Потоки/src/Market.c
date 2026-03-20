#include "Market.h"

extern int AllCustomersIsFull;

void StartMarket(struct Customer* customers, struct Loader* loader){
	printf("Рынок открывыется!\n\n");

	for (int i=0; i<COUNT_CUSTOMERS; i++) {
		pthread_create(&(customers[i].thread), NULL, Eat, &(customers[i]));
	}
	pthread_create(&(loader->thread), NULL, LoadUp, loader);
}

void EndMarket(struct Customer* customers, struct Loader* loader){
	for (int i=0; i<COUNT_CUSTOMERS; i++) {
		pthread_join(customers[i].thread, NULL);
	}
	AllCustomersIsFull = 1;
	pthread_join(loader->thread, NULL);
	printf("Всё наелись. Рынок закрывается!\n\n");
}
