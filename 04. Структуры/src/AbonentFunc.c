#include "AbonentFunc.h"

void PrintAbonent(struct abonent *abo){
	if(abo == NULL){
		printf("Ошибка: указатель на структуру abonent равен NULL\n");
		return;
	}
	printf("%s %s - %s\n", abo->name, abo->second_name, abo->tel);
}

void PrintVector(struct vector *vec){
	if(vec == NULL){
		printf("Ошибка: указатель на структуру vector равен NULL\n");
		return;
	}
	int size = vec->current_size;
	for(int i=0; i<size; i++){
		struct abonent abo = vec->abonents[i];
		printf("%d. ", (i+1));
		PrintAbonent(&abo);
	}
	printf("\n");
}

void PrintVectorInRange(struct vector *vec, unsigned int L, unsigned int R){
	if(vec == NULL){
		printf("Ошибка: указатель на структуру vector равен NULL\n");
		return;
	}
	int size = vec->current_size;
	if(L>R){
		printf("Ошибка: L > R\n");
		return;
	}
	if(R>size){
		printf("Ошибка: R > size\n");
		return;
	}
	for(int i=L; i<R; i++){
		struct abonent abo = vec->abonents[i];
		printf("%d. ", (i+1));
		PrintAbonent(&abo);
	}
	printf("\n");
}

void InitVector(struct vector *vec){
	if(vec == NULL){
		printf("Ошибка: указатель на структуру vector равен NULL\n");
		return;
	}
	vec->capacity = SIZE_ABONENTS;
	vec->current_size = 0;
}

int AddAbonent(struct vector *vec, char* aname, char* asecond_name, char *atel){
	if(vec == NULL){
		printf("Ошибка: указатель на структуру vector равен NULL\n");
		return 1;
	}
	int size = vec->current_size;
	if(size >= vec->capacity){
		printf("Ошибка: абонентов слишком много, удалите лишних\n");
		return 1;
	}
	strncpy(vec->abonents[size].name, aname, SIZE_CHAR_ARRAY);
	strncpy(vec->abonents[size].second_name, asecond_name, SIZE_CHAR_ARRAY);
	strncpy(vec->abonents[size].tel, atel, SIZE_CHAR_ARRAY);
	vec->current_size++;
	return 0;
}

int DeleteAbonentId(struct vector *vec, unsigned int id){
	if(vec == NULL){
		printf("Ошибка: указатель на структуру vector равен NULL\n");
		return 1;
	}
	int size = vec->current_size;

	if (id >= size){
		printf("Ошибка: абонента с указанным id не существует\n");
		return 1;
	}

	for (int i = id; i < size; i++){
		vec->abonents[i] = vec->abonents[i+1];
	}

	vec->current_size--;
	return 0;
}

struct abonent* SearchAbonentName(struct vector *vec, char *aname){
	if(vec == NULL){
		printf("Ошибка: указатель на структуру vector равен NULL\n");
		return NULL;
	}
	int size = vec->current_size;
	for(int i=0; i<size; i++){
		char current_name[SIZE_CHAR_ARRAY];
		if(strcmp(aname, vec->abonents[i].name) == 0){
			return &vec->abonents[i];
		}
	}
	return NULL;
}
