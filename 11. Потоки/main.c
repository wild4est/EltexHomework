#include "src/Market.h"

int main(){
	struct Customer* customers = malloc(sizeof(struct Customer) * COUNT_CUSTOMERS);
	struct Stall* stalls = malloc(sizeof(struct Stall) * COUNT_STALLS);
	struct Loader* loader = malloc(sizeof(struct Loader));

	InitStalls(stalls);
	InitCustomers(customers, stalls);
	InitLoader(loader, stalls);

	StartMarket(customers, loader);
	EndMarket(customers, loader);
	return 0;
}
