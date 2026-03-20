#include "Stall.h"

void InitStalls(struct Stall* stalls){
	printf("Инициализируются палатки:\n");
	for (int i=0; i<COUNT_STALLS; i++) {
		stalls[i].id = i;
		stalls[i].count_product = rand()%200 + COUNT_PRODUCT;
		pthread_mutex_init(&(stalls[i].mutex), NULL);
		printf("Палатка[%d] готова принимать клиентов\n", stalls[i].id);
	}
	printf("\n");
}

