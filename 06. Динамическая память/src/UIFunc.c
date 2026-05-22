#include "UIFunc.h"

void AlternateScreenOn(){
	printf("\033[?1049h\033[H");
}

void AlternateScreenOff(){
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

void PrintAbonentMenu(struct ListNode *first_node_on_page, int page, int current_size){
	if(current_size==0){
		printf("Вектор пуст\n");
		return;
	}
	PrintListInRange(first_node_on_page, STEP);
	printf("(%d / %d )\n", (page/STEP + 1), ((current_size + STEP - 1)/STEP));

}


void AddAbonentMenu(struct List *list){
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

	struct Abonent* abonent = InitAbonent(name, second_name, tel);
	int result = AddAbonent(list, abonent);
	Clear();
	if(!result){
		printf("Абонент успешно добавлен\n");
	}else{
		printf("Что-то пошло не так и абонент не добавлен\n");
	}
}


void DeleteAbonentMenu(struct List* list){
	if(list->current_size == 0){
		printf("Ошибка: Вектор пуст. Удалять нечего.\n");
		return;
	}

	Clear();
	printf("(Удаление) Введите id (если хотите отменить введите -1):");
	int id;
	scanf("%d", &id);
	if(id == -1){
		return;
	}

	int result = DeleteAbonentId(list, id);
	Clear();
	if(!result){
		printf("Абонент успешно удалён\n");
	}else{
		printf("Что-то пошло не так и абонент не удалён\n");
	}
}

void SearchAbonentMenu(struct List* list){
	Clear();
	printf("(Поиск) Введите имя (если хотите отменить введите -1):");
	char name[SIZE_CHAR_ARRAY];
	scanf("%9s", name);
	scanf("%*[^\n]%*c");
	if(strcmp(name, "-1")==0){
		return;
	}

	struct Abonent* result = SearchAbonentName(list, name);
	Clear();
	if(result != NULL){
		printf("Найденный абонент:");
		PrintAbonent(result);
	}else{
		printf("Абонент не найден\n");
	}
}


void Output(int get, int* page, struct ListNode** first_node_on_page, struct List* list){
	struct ListNode* tmp;
	switch(get){
		case ADD:
			AddAbonentMenu(list);
			*first_node_on_page = list->start;
			*page = 0;
			break;
		case DELETE:
			DeleteAbonentMenu(list);
			*first_node_on_page = list->start;
			*page = 0;
			break;
		case SEARCH:
			SearchAbonentMenu(list);
			break;
		case LEFT:
			tmp = *first_node_on_page;
			for(int i=0; i<STEP; i++){
				if(*first_node_on_page==NULL){
					*first_node_on_page = tmp;
					return;
				}
				*first_node_on_page = (*first_node_on_page)->prev;
			}
			*page-=STEP;
			break;
		case RIGHT:
			tmp = *first_node_on_page;
			for(int i=0; i<STEP; i++){
				if(*first_node_on_page==NULL){
					*first_node_on_page = tmp;
					return;
				}
				*first_node_on_page = (*first_node_on_page)->next;
			};
			*page+=STEP;
			break;
	}
}


void MainScreen(){

	struct List *list = InitList();

	AlternateScreenOn();

	int get;
	int page = 0;
	struct ListNode* first_node_on_page = NULL;
	do {
		PrintAbonentMenu(first_node_on_page, page, list->current_size);
		PrintMenu();
		scanf("%d", &get);
		Clear();
		Output(get, &page, &first_node_on_page, list);
	} while (get != EXIT);

	AlternateScreenOff();
}
