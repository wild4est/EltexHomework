#include "PointerFunc.h"


int ReplaceThirdByte(int num, unsigned char num_replaced){
	unsigned char* num_pointer = (unsigned char*) &num;
	num_pointer+=2;
	*num_pointer = num_replaced;
	return num;
}

void Result12(void){
	float x = 5.0;
	printf("x = %f, ", x);
	float y = 6.0;
	printf("y = %f\n", y);
	float *xp = &y;
	float *yp = &y;
	printf("Результат: %f\n", *xp + *yp);
}

void FillArray(int* array, int n){
	for(int i=0; i<n; i++){
		*array = i+1;
		array++;
	}
}

void PrintArrayInt(int* array, int n){
	for(int i=0; i<n; i++){
		printf("%d ", *array);
		array++;
	}
	printf("\n");
}

void PrintArrayChar(const char* array, int n){
	for(int i=0; i<n; i++){
		printf("%c", *array);
		array++;
	}
	printf("\n");
}

int GetStringSize(const char* str){
	int size=0;
	while(*str != '\0'){
		size++;
		str++;
	}
	return size;

}

const char* BitapAlgorithm(const char* text, const char* pattern, int text_size, int pattern_size){
	int alphabet_mask_size = 127;
	
	if(pattern_size == 0){
		return text;
	}

	if(pattern_size > (sizeof(unsigned long)*8 - 1)){
		printf("Подстрока слишком большая\n");
		return NULL;
	}

	// Битовый массив
	unsigned long R = ~1;
	// Массив масок алфавита
	unsigned long alphabet_mask[alphabet_mask_size+1];
	unsigned long unit = 1;

	// Заполнение массива масок единицами
	for(int i=0; i<=alphabet_mask_size; i++){
		alphabet_mask[i] = ~0;
	}

	// Отображение положение каждого символа в подстроке
	for(int i=0; i<pattern_size; i++){
		alphabet_mask[*pattern] &= ~(unit << i);
		pattern++;
	}

	for(int i=0; i<text_size; i++){
		// Наложение маски текущей буквы строки на битовый массив
		R |= alphabet_mask[*text];
		R <<= 1;

		// Проверка положения *ведущего нуля*
		if ((R & (unit << pattern_size)) == 0){
			return (text - pattern_size + 1);
		}
		text++;
	}

	return NULL;
}
