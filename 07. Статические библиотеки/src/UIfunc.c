#include "UIfunc.h"

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
         printf("Введите один из пунктов меню: (1/2/3/4/5)\n");
         printf("1. Сложение\n");
         printf("2. Вычитание\n");
         printf("3. Умножение\n");
         printf("4. Деление\n");
         printf("5. Выход\n");
}

void AddMenu(){
	Clear();
	printf("(Сложение) Введите первое слагаемое:\n");
	int a;
	scanf("%d", &a);

	Clear();
	printf("(Сложение) Введите второе слагаемое:\n");
	int b;
	scanf("%d", &b);

	Clear();
	int answer;
	add(&answer, a, b);
	printf("Ваш ответ:\n %d + %d = %d\n", a, b, answer);
}

void SubMenu(){
	Clear();
	printf("(Вычитание) Введите уменьшаемое:\n");
	int a;
	scanf("%d", &a);

	Clear();
	printf("Вычитание) Введите вычитаемое:\n");
	int b;
	scanf("%d", &b);

	Clear();
	int answer;
	sub(&answer, a, b);
	printf("Ваш ответ:\n %d - %d = %d\n", a, b, answer);
}

void MulMenu(){
	Clear();
	printf("(Умножение) Введите первый множитель:\n");
	int a;
	scanf("%d", &a);

	Clear();
	printf("(Умножение) Введите второй множитель:\n");
	int b;
	scanf("%d", &b);

	Clear();
	int answer;
	mul(&answer, a, b);
	printf("Ваш ответ:\n %d * %d = %d\n", a, b, answer);
}

void DivMenu(){
	Clear();
	printf("(Деление) Введите делимое:\n");
	int a;
	scanf("%d", &a);

	Clear();
	printf("(Деление) Введите делитель:\n");
	int b;
	scanf("%d", &b);

	Clear();
	int answer;
	int result = div(&answer, a, b);
	if(result==1){
		printf("Делить на ноль нельзя\n");	
	}else{
		printf("Ваш ответ:\n %d * %d = %d\n", a, b, answer);
	}
}


void Output(int get){
	switch(get){
		case ADD:
			AddMenu();
			break;
		case SUB:
			SubMenu();
			break;
		case MUL:
			MulMenu();
			break;
		case DIV:
			DivMenu();
			break;
	}
}

void MainScreen(){
	AlternateScreenOn();
	int get;
	do {
		PrintMenu();
		scanf("%d", &get);
		Clear();
		Output(get);
	} while (get != EXIT);
	AlternateScreenOff();
}
