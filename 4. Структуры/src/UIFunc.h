#include "AbonentFunc.h"

#define STEP 5

enum condition{
	ADD = 1,
	DELETE,
	SEARCH,
	LEFT,
	RIGHT,
	EXIT
};

void AlternativeScreenOn();
void AlternativeScreenOff();
void Clear();
void PrintMenu();
void PrintAbonentMenu(struct vector *vec, int *page);
void AddAbonentMenu(struct vector *vec);
void DeleteAbonentMenu(struct vector *vec);
void SearchAbonentMenu(struct vector *vec);
void Output(int get, int* page, struct vector *vec);
void MainScreen();
