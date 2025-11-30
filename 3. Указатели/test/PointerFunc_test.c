#include "../src/PointerFunc.h"
#include "../../Unity/unity.h"


void setUp(){

}


void tearDown(){

}

void ReplaceThirdByte_test(){
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(255,255),0xFF00FF);
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(5,9),0x090005);
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(0xFFFFFF,0),0x00FFFF);
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(0x910000,0x5B),0x5B0000);
}


void FillArray_test(){
	int N1 = 4;
	int buf1[] = {1,2,3,4};
	int array1[N1];
	FillArray(&array1[0], N1);
	TEST_ASSERT_EQUAL_INT_ARRAY(array1, buf1, N1);

	int N2 = 7;
	int buf2[] = {1,2,3,4,5,6,7};
	int array2[N2];
	FillArray(&array2[0], N2);
	TEST_ASSERT_EQUAL_INT_ARRAY(array2, buf2, N2);

	int N3 = 10;
	int buf3[] = {1,2,3,4,5,6,7,8,9,10};
	int array3[N3];
	FillArray(&array3[0], N3);
	TEST_ASSERT_EQUAL_INT_ARRAY(array3, buf3, N3);
}

void BitapAlgorithm_test(){
	const char* text = "RosesAreRedVioletsAreBlue";

	const char* pattern1 = "Blue";
	const char* point_substr1 = BitapAlgorithm(text, pattern1, 25, 4);
	TEST_ASSERT_EQUAL_PTR(point_substr1, text+21);

	const char* pattern2 = "Red";
	const char* point_substr2 = BitapAlgorithm(text, pattern2, 25, 3);
	TEST_ASSERT_EQUAL_PTR(point_substr2, text+8);

	const char* pattern3 = "Roses";
	const char* point_substr3 = BitapAlgorithm(text, pattern3, 25, 5);
	TEST_ASSERT_EQUAL_PTR(point_substr3, text);

	const char* pattern4 = "Green";
	const char* point_substr4 = BitapAlgorithm(text, pattern4, 25, 5);
	TEST_ASSERT_EQUAL_PTR(point_substr4, NULL);
}

int main(){
	UNITY_BEGIN();
	RUN_TEST(ReplaceThirdByte_test);
	RUN_TEST(FillArray_test);
	RUN_TEST(BitapAlgorithm_test);
	return UNITY_END();
}
