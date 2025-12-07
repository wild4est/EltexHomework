#include "UIFunc.h"

void AlternativeScreenOn(){
	printf("\033[?1049h\033[H");
}

void AlternativeScreenOff(){
	printf("\033[?1049l");
}

void Clear(){
	printf("\033[H\033[J");
}

void PrintMenu(){
	printf("-----------------------------------------------\n");
	printf("Введите один из пунктов меню: (1/2/3/4/5/6)\n");
	printf("1. Добавить абонента\n");
	printf("2. Удалить абонента\n");
	printf("3. Поиск абонента\n");
	printf("4. Перелистнуть налево\n");
	printf("5. Перелистнуть направо\n");
	printf("6. Выход\n");
	
}

void PrintAbonentMenu(struct vector *vec, int *page){
	if(vec->current_size==0){
		printf("Вектор пуст\n");
		return;
	}
	
	if(*page >= vec->current_size){
		*page -= STEP;
	}
	
	if(*page < 0){
		*page = 0;
	}

	if((*page+STEP) >= vec->current_size){
		PrintVectorInRange(vec, *page, vec->current_size);
	}else{
		PrintVectorInRange(vec, *page, (*page+STEP));
	}
	
	printf("(%d / %d )\n", (*page/STEP + 1), ((vec->current_size + STEP - 1)/STEP));

}


void AddAbonentMenu(struct vector *vec){
	Clear();
	printf("(Добавление) Введите имя (если хотите отменить введите -1):");
	char name[SIZE_CHAR_ARRAY];
	scanf("%9s", name);
	scanf("%*[^\n]%*c");
	if(strcmp(name, "-1")==0){
		return;
	}
	
	Clear();
	printf("(Добавление) Введите фамилия (если хотите отменить введите -1):");
	char second_name[SIZE_CHAR_ARRAY];
	scanf("%9s", second_name);
	scanf("%*[^\n]%*c");
	if(strcmp(second_name, "-1")==0){
		return;
	}

	Clear();
	printf("(Добавление) Введите телефон (если хотите отменить введите -1):");
	char tel[SIZE_CHAR_ARRAY];
	scanf("%9s", tel);
	scanf("%*[^\n]%*c");
	if(strcmp(tel, "-1")==0){
		return;
	}

	int result = AddAbonent(vec, name, second_name, tel);
	Clear();
	if(!result){
		printf("Абонент успешно добавлен\n");
	}else{
		printf("Что-то пошло не так и абонент не добавлен\n");
	}

}


void DeleteAbonentMenu(struct vector *vec){
	if(vec->current_size == 0){
		printf("Ошибка: Вектор пуст. Удалять нечего.\n");
		return;
	}

	Clear();
	printf("(Удаление) Введите имя (если хотите отменить введите -1):");
	int id;
	scanf("%d", &id);
	if(id == -1){
		return;
	}

	int result = DeleteAbonentId(vec, id);
	Clear();
	if(!result){
		printf("Абонент успешно удалён\n");
	}else{
		printf("Что-то пошло не так и абонент не удалён\n");
	}
}

void SearchAbonentMenu(struct vector *vec){
	Clear();
	printf("(Поиск) Введите имя (если хотите отменить введите -1):");
	char name[SIZE_CHAR_ARRAY];
	scanf("%9s", name);
	scanf("%*[^\n]%*c");
	if(strcmp(name, "-1")==0){
		return;
	}

	struct abonent* result = SearchAbonentName(vec, name);
	Clear();
	if(result != NULL){
		printf("Найденный абонент:");
		PrintAbonent(result);
	}else{
		printf("Абонент не найден\n");
	}
}


void Output(int get, int* page, struct vector *vec){
	switch(get){
		case ADD:
			AddAbonentMenu(vec);
			break;
		case DELETE:
			DeleteAbonentMenu(vec);
			break;
		case SEARCH:
			SearchAbonentMenu(vec);
			break;
		case LEFT:
			*page-=STEP;
			break;
		case RIGHT:
			*page+=STEP;
			break;
	}
}


void MainScreen(){

	struct vector vec;
	InitVector(&vec);

	AlternativeScreenOn();

	int get;
	int page = 0;
	do {
		PrintAbonentMenu(&vec, &page);
		PrintMenu();
		scanf("%d", &get);
		Clear();
		Output(get, &page, &vec);
	} while (get != EXIT);

	AlternativeScreenOff();
}
