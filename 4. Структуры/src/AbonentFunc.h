#include <stdio.h>
#include <string.h>
#define SIZE_CHAR_ARRAY 10
#define SIZE_ABONENTS 100

struct abonent {
	char name[SIZE_CHAR_ARRAY];
	char second_name[SIZE_CHAR_ARRAY];
	char tel[SIZE_CHAR_ARRAY];
};

struct vector {
	int capacity;
	int current_size;
	struct abonent abonents[SIZE_ABONENTS];
};

void PrintAbonent(struct abonent *abo);
void PrintVector(struct vector *vec);
void PrintVectorInRange(struct vector *vec, unsigned int L, unsigned int R);
void InitVector(struct vector *vec);
int AddAbonent(struct vector *vec, char *aname, char *asecond_name, char *atel);
int DeleteAbonentId(struct vector *vec, unsigned int id);
struct abonent* SearchAbonentName(struct vector *vec, char *aname);
