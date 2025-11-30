#include "../src/ArrayFunc.h"
#include "../../Unity/unity.h"

void setUp(void){

}

void tearDown(void){

}

void FillMatrix_test(void){
	int N=3, M=4;
	int buff1[3][3] = {{1,2,3},
			{4,5,6},
			{7,8,9}};
	int matrix1[N][N];
	FillMatrix(&matrix1[0][0], N, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix1, buff1, N*N);

	int buff2[4][4] = {{1,2,3,4},
			{5,6,7,8},
			{9,10,11,12},
			{13,14,15,16}};
	int matrix2[M][M];
	FillMatrix(&matrix2[0][0], M, M);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix2, buff2, M*M);
	
	int buff3[3][4] = {{1,2,3,4},
			{5,6,7,8},
			{9,10,11,12}};
	int matrix3[N][M];
	FillMatrix(&matrix3[0][0], N, M);

	TEST_ASSERT_EQUAL_INT_ARRAY(matrix3, buff3, N*M);
	
	int buff4[4][3] = {{1,2,3},
			{4,5,6},
			{7,8,9},
			{10,11,12}};
	int matrix4[M][N];
	FillMatrix(&matrix4[0][0], M, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix4, buff4, N*M);
}

void FillTriangularMatrix_test(void){
	int N=3, M=4;
	int buff1[3][3] = {{0,0,1},
			{0,1,1},
			{1,1,1}};
	int matrix1[N][N];
	FillTriangularMatrix(&matrix1[0][0], N, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix1, buff1, N*N);

	int buff2[4][4] = {{0,0,0,1},
			{0,0,1,1},
			{0,1,1,1},
			{1,1,1,1}};
	int matrix2[M][M];
	FillTriangularMatrix(&matrix2[0][0], M, M);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix2, buff2, M*M);
	
	int buff3[3][4] = {{0,0,0,1},
			{0,0,1,1},
			{0,1,1,1}};
	int matrix3[N][M];
	FillTriangularMatrix(&matrix3[0][0], N, M);

	TEST_ASSERT_EQUAL_INT_ARRAY(matrix3, buff3, N*M);
	
	int buff4[4][3] = {{0,0,0},
			{0,0,1},
			{0,1,1},
			{1,1,1}};
	int matrix4[M][N];
	FillTriangularMatrix(&matrix4[0][0], M, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix4, buff4, N*M);


}

void Swap_test(void){
	int a1=5, b1=6;
	Swap(&a1, &b1);
	TEST_ASSERT_EQUAL_INT(a1, 6);
	TEST_ASSERT_EQUAL_INT(b1, 5);

	int a2=5, b2=5;
	Swap(&a2, &b2);
	TEST_ASSERT_EQUAL_INT(a2, 5);
	TEST_ASSERT_EQUAL_INT(b2, 5);

	int a3=-5, b3=-6;
	Swap(&a3, &b3);
	TEST_ASSERT_EQUAL_INT(a3, -6);
	TEST_ASSERT_EQUAL_INT(b3, -5);
}

void FillArray_test(void){
	int N1 = 4;
	int buf1[] = {1,2,3,4};
	int array1[N1];
	FillArray(array1, N1);
	TEST_ASSERT_EQUAL_INT_ARRAY(array1, buf1, N1);

	int N2 = 7;
	int buf2[] = {1,2,3,4,5,6,7};
	int array2[N2];
	FillArray(array2, N2);
	TEST_ASSERT_EQUAL_INT_ARRAY(array2, buf2, N2);

	int N3 = 10;
	int buf3[] = {1,2,3,4,5,6,7,8,9,10};
	int array3[N3];
	FillArray(array3, N3);
	TEST_ASSERT_EQUAL_INT_ARRAY(array3, buf3, N3);
}

void ReverseArray_test(void){
	int N1 = 4;
	int buf1[] = {1,2,3,4};
	int buf1_rev[] = {4,3,2,1};
	ReverseArray(buf1, N1);
	TEST_ASSERT_EQUAL_INT_ARRAY(buf1, buf1_rev, N1);

	int N2 = 7;
	int buf2[] = {1,2,3,4,5,6,7};
	int buf2_rev[] = {7,6,5,4,3,2,1};
	ReverseArray(buf2, N2);
	TEST_ASSERT_EQUAL_INT_ARRAY(buf2, buf2_rev, N2);

	int N3 = 10;
	int buf3[] = {1,2,3,4,5,6,7,8,9,10};
	int buf3_rev[] = {10,9,8,7,6,5,4,3,2,1};
	ReverseArray(buf3, N3);
	TEST_ASSERT_EQUAL_INT_ARRAY(buf3, buf3_rev, N3);
}

void FillSnailMatrix_test(void){
	int N=3, M=4;
	int buff1[3][3] = {{1,2,3},
			{8,9,4},
			{7,6,5}};
	int matrix1[N][N];
	FillSnailMatrix(&matrix1[0][0], N, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix1, buff1, N*N);

	int buff2[4][4] = {{1,2,3,4},
			{12,13,14,5},
			{11,16,15,6},
			{10,9,8,7}};
	int matrix2[M][M];
	FillSnailMatrix(&matrix2[0][0], M, M);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix2, buff2, M*M);
	
	int buff3[3][4] = {{1,2,3,4},
			{10,11,12,5},
			{9,8,7,6}};
	int matrix3[N][M];
	FillSnailMatrix(&matrix3[0][0], N, M);

	TEST_ASSERT_EQUAL_INT_ARRAY(matrix3, buff3, N*M);
	
	int buff4[4][3] = {{1,2,3},
			{10,11,4},
			{9,12,5},
			{8,7,6}};
	int matrix4[M][N];
	FillSnailMatrix(&matrix4[0][0], M, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix4, buff4, N*M);
}


void FillZeroMatrix_test(void){
	int N=3, M=4;
	int buff1[3][3] = {{0,0,0},
			{0,0,0},
			{0,0,0}};
	int matrix1[N][N];
	FillZeroMatrix(&matrix1[0][0], N, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix1, buff1, N*N);

	int buff2[4][4] = {{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}};
	int matrix2[M][M];
	FillZeroMatrix(&matrix2[0][0], M, M);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix2, buff2, M*M);
	
	int buff3[3][4] = {{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}};
	int matrix3[N][M];
	FillZeroMatrix(&matrix3[0][0], N, M);

	TEST_ASSERT_EQUAL_INT_ARRAY(matrix3, buff3, N*M);
	
	int buff4[4][3] = {{0,0,0},
			{0,0,0},
			{0,0,0},
			{0,0,0}};
	int matrix4[M][N];
	FillZeroMatrix(&matrix4[0][0], M, N);
	
	TEST_ASSERT_EQUAL_INT_ARRAY(matrix4, buff4, N*M);


}


int main(){
	UNITY_BEGIN();
	RUN_TEST(FillMatrix_test);
	RUN_TEST(FillTriangularMatrix_test);
	RUN_TEST(Swap_test);
	RUN_TEST(FillArray_test);
	RUN_TEST(ReverseArray_test);
	RUN_TEST(FillSnailMatrix_test);
	RUN_TEST(FillZeroMatrix_test);
	return UNITY_END();
}
