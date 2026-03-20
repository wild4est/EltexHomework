#include "Customer.h"

void InitCustomers(struct Customer* customers, struct Stall* stalls){
	printf("Инициализируются покупатели:\n");
	for (int i=0; i<COUNT_CUSTOMERS; i++) {
		customers[i].id = i;
		customers[i].hunger = rand()%200 + COUNT_HUNGER;
		customers[i].stalls = stalls;
		printf("Покупатель[№%d] голод = %d готов к покупкам\n", customers[i].id, customers[i].hunger);
	}
	printf("\n");
}

void* Eat(void* args){
	struct Customer* customer = (struct Customer*)args;
	
	while (customer->hunger > 0){
		printf("Покупатель[%d] всё ещё голоден. Он ищет палатку.\n", customer->id);
		int i = rand()%COUNT_STALLS;
		if (pthread_mutex_trylock(&(customer->stalls[i].mutex))==0) {
			printf("Покупатель[%d] зашёл в палатку[%d]\n", customer->id, customer->stalls[i].id);
			if (customer->hunger > customer->stalls[i].count_product) {
				printf("Покупатель[%d] скупил %d пирожков. Теперь его голод составляет %d\n",
					customer->id, customer->stalls[i].count_product, customer->hunger);
				customer->hunger -= customer->stalls[i].count_product;
				customer->stalls[i].count_product = 0;
			}else{
				printf("Покупатель[%d] скупил %d пирожков. Теперь его голод составляет %d\n",
					customer->id, customer->hunger, 0);
				customer->stalls[i].count_product -= customer->hunger;
				customer->hunger = 0;
			
			}
			pthread_mutex_unlock(&(customer->stalls[i].mutex));
			sleep(2);
		}
	}
	printf("!!! Покупатель[%d] наконец-то наелся и пошёл домой !!!\n", customer->id);
	return NULL;
}
