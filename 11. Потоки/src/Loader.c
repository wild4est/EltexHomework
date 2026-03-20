#include "Loader.h"

int AllCustomersIsFull = 0;

void InitLoader(struct Loader* loader, struct Stall* stalls){
	printf("Инициализируется грузчик Пётр.\n");
	loader->load = COUNT_LOAD;
	loader->stalls = stalls;
	printf("Грузчик Пётр готов грузить товар\n\n");
}

void* LoadUp(void* args){
	struct Loader* loader = (struct Loader*)args;
	
	while (AllCustomersIsFull != 1) {
		printf("Грузчик Пётр проснулся и думает в какую палатку ему заглянуть.\n");
		int chosen_stall = rand()%COUNT_STALLS;
		printf("Грузчик Пётр выбрал палатку[%d]\n", chosen_stall);
		if (pthread_mutex_trylock(&(loader->stalls[chosen_stall].mutex))==0) {
			printf("Грузчик Пётр заглянул в палатку[%d]. В ней не оказалось посетителей.\n", chosen_stall);
			printf("Грузчик Пётр загрузил %d пирожков в палатку[%d] и ушёл спать.\n",
					loader->load, chosen_stall);
			loader->stalls[chosen_stall].count_product += loader->load;
			pthread_mutex_unlock(&(loader->stalls[chosen_stall].mutex));
			sleep(1);
		}else{
			printf("Грузчик Пётр заглянул в палатку[%d], но она оказалась занята...\n", chosen_stall);
		}
	}

	return NULL;
}
