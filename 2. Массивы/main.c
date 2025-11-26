#include "src/ArrayFunc.h"

#define N 5
#define M 4


int main(){
	printf("N = %d\n", N);
	
	printf("1. Вывести квадратную матрицу по заданному N.\n");
	int matrix[N][M];
	FillMatrix(&matrix[0][0], N, M);
	PrintMatrix(&matrix[0][0], N, M);
	printf("\n");
	
	printf("2. Вывести заданный массив размером N в обратном порядке.\n");
	int array[N];
	FillArray(array, N);
	ReverseArray(array, N);
	PrintArray(array, N);
	printf("\n");

	printf("3. Заполнить верхний треугольник матрицы 1, а нижний 0.\n");
	int triangular_matrix[N][M];
	FillTriangularMatrix(&triangular_matrix[0][0], N, M);
	PrintMatrix(&triangular_matrix[0][0], N, M);
	printf("\n");

	printf("4. Заполнить матрицу числами от 1 до N2 улиткой.\n");
	int snail_matrix[N][M];
	FillSnailMatrix(&snail_matrix[0][0], N, M);
	PrintMatrix(&snail_matrix[0][0], N, M);
}
