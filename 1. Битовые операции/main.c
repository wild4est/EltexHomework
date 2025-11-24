#include <stdio.h>
#include "src/BinaryFunc.h"

int main(){
	printf("1. Вывести двоичное представление целого положительного числа, используя битовые операции (число вводится с клавиатуры).\n");

	int num_pos;
	scanf("%d", &num_pos);
	
	char bin_num[34];
	toBinary(num_pos, bin_num);
	
	printf("Двоичное представление числа %d : %s\n", num_pos, bin_num);
	
	
	printf("\n2. Вывести двоичное представление целого отрицательного числа, используя битовые операции (число вводится с клавиатуры).\n");
	int num_neg;
	scanf("%d", &num_neg);

	char bin_num_neg[34];
	toBinary(num_neg, bin_num_neg);

	printf("Двоичное представление числа %d : %s\n", num_neg, bin_num_neg);


	printf("\n3. Найти количество единиц в двоичном представлении целого положительного числа (число вводится с клавиатуры).\n");
	int num;
	scanf("%d", &num);
	printf("Колличество единиц в числе %d: %d\n", num, countBits(num));


	printf("\n4. Поменять в целом положительном числе (типа int) значение третьего байта на введенное пользователем число (изначальное число также вводится с клавиатуры).\n");
	int num_original, num_replaced;
	printf("Введите первоначальное число: ");
	scanf("%d", &num_original);
	printf("Введите число, на которое хотите изменить 3 байт: ");
	scanf("%d", &num_replaced);
	
	printf("Итоговое число: %d\n", replaceThirdByte(num_original, num_replaced));
	

	return 0;
}
