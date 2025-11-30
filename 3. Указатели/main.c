#include "src/PointerFunc.h"


int main(){
	printf("1. Поменять в целом положительном числе (типа int) значение третьего\n"
		"байта на введенное пользователем число (изначальное число также\n"
		"вводится с клавиатуры) через указатель (не применяя битовые операции).\n\n");
	printf("Введите первоначальное число: ");
	int num_original;
	scanf("%d", &num_original);

	printf("Введите число, на которое хотите заменить 3 байт: ");
	int num_replaced;
	scanf("%d", &num_replaced);

	printf("Итоговое число: %d\n",ReplaceThirdByte(num_original, num_replaced));

	printf("\n2. В приведенном ниже коде измените только одну строку (помеченную),\n"
		"чтобы напечатать “Результат: 12.0”\n\n");

	Result12();
	
	printf("\n3. Напишите программу, в которой определите массив типа int (размер\n"
		"массива 10 элементов от 1 до 10). Используйте указатель для вывода\n"
		"элементов массива на консоль (применять можно только арифметику\n"
		"указателей, а не индексы).\n\n");

	int N = 10;
	int array[N];
	FillArray(&array[0], N);
	PrintArrayInt(&array[0], N);

	printf("4. Напишите программу, которая ищет введенной строке (с клавиатуры)\n"
		"введенную подстроку (с клавиатуры) и возвращает указатель на начало\n"
		"подстроки, если подстрока не найдена в указатель записывается NULL.\n"
		"В качестве срок использовать статические массивы.\n\n");

	printf("Введите строку: ");
	char text[50];
	scanf("%49s", text);

	printf("Введите подстроку: ");
	char pattern[50];
	scanf("%49s", pattern);

	int text_size = GetStringSize(text);
	int pattern_size = GetStringSize(pattern);
	const char* point_substring = BitapAlgorithm(text, pattern, text_size, pattern_size);
	printf("Найденная подстрока: \n");
	PrintArrayChar(point_substring, pattern_size);
	printf("Индекс подстроки: %ld\n", point_substring-text);
}
