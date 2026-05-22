#include "ArrayFunc.h"

void PrintArray(int* array, unsigned int N){
	for(int i=0; i<N; i++){
		printf("%d ", array[i]);
	}
	printf("\n");
}

void PrintMatrix(int* matrix, unsigned int N, unsigned int M){
	for(int i=0; i<N; i++){
		PrintArray(matrix+(i*M), M);
	}
}

void FillMatrix(int* matrix, unsigned int N, unsigned int M){
	int num = 1;
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			matrix[i*M + j] = num;
			num++;
		}
	}
}


void FillTriangularMatrix(int* matrix, unsigned int N, unsigned int M){
	int boundary = (N >= M) ? (N-1) : (M-1);
	
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			if((i+j) >= boundary){
				matrix[i*M + j] = 1;
				continue;
			}
			matrix[i*M + j] = 0;
		}
	}
}

void Swap(int* a, int* b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void FillArray(int* array, unsigned int N){
	for(int i=0; i<N; i++){
		array[i] = i+1;
	}
}

void ReverseArray(int* array, unsigned int N){
	int middle = N >> 1;

	for(int i=0; i<middle; i++){
		Swap(&array[i], &array[N-1-i]);
	}
}

void FillZeroMatrix(int* matrix, unsigned int N, unsigned int M){
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			matrix[i*M+j] = 0;
		}
	}

}

void FillSnailMatrix(int* matrix, unsigned int N, unsigned int M){
	int num = 1;
	int x0 = 0, y0 = 0; // левый верхний угол матрицы
	int x1 = M, y1 = N; // правый нижний угол матрицы
	int sign1 = -1;
	int sign2 = 1;

	while(num <= M*N){
		for(int j=x0; j<x1; j++){ //заполнение верхней строки
			*matrix = num;
			num++;
			matrix++;
		}
		matrix--;
	

		for(int i=y0; i<y1-1; i++){ //заполнение крайнего правого столбца
			matrix+=M;
			*matrix = num;
			num++;
		}
		matrix--;
		
		if(num>M*N){
			break;
		}
		
		for(int j=x0; j<x1-1; j++){ //заполнение нижней строки
			*matrix=num;
			num++;
			matrix--;
		}
		matrix++;
		
		
		for(int i=y0; i<y1-2; i++){ //заполнение крайней левой строки
			matrix-=M;
			*matrix = num;
			num++;
		}
		matrix++;

		//переход к следующему кругу
		x0++;
		y0++;
		x1--;
		y1--;
	}
}
