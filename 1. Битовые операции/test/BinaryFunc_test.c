#include "../../Unity/unity.h"
#include "../src/BinaryFunc.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void PowOfTwo_test(void){
	TEST_ASSERT_EQUAL_INT(PowOfTwo(1),1);
	TEST_ASSERT_EQUAL_INT(PowOfTwo(2),2);
	TEST_ASSERT_EQUAL_INT(PowOfTwo(5),3);
	TEST_ASSERT_EQUAL_INT(PowOfTwo(9),4);
}

void ReverseMasChar_test(void){
	char buff1[] = "abcde";
	char rev_buff1[5];
	ReverseMasChar(buff1, rev_buff1, 5);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(rev_buff1, "edcba", 5);

	char buff2[] = "hello world";
	char rev_buff2[11];
	ReverseMasChar(buff2, rev_buff2, 11);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(rev_buff2, "dlrow olleh", 11);

	char buff3[] = "123";
	char rev_buff3[3];
	ReverseMasChar(buff3, rev_buff3, 3);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(rev_buff3, "321", 3);
}


void IntToBinary_test_pos(void){
	int num1 = 10;
	char buff1[32];
	IntToBinary(num1, buff1);
	TEST_ASSERT_EQUAL_STRING(buff1, "1010");
	
	int num2 = 30;
	char buff2[32];
	IntToBinary(num2, buff2);
	TEST_ASSERT_EQUAL_STRING(buff2, "11110");

	int num3 = 56;
	char buff3[32];
	IntToBinary(num3, buff3);
	TEST_ASSERT_EQUAL_STRING(buff3, "111000");

}

void IntToBinary_test_neg(void){
	int num1 = -10;
	char buff1[32];
	IntToBinary(num1, buff1);
	TEST_ASSERT_EQUAL_STRING(buff1, "11111111""11111111""11111111""11110110");
	
	int num2 = -30;
	char buff2[32];
	IntToBinary(num2, buff2);
	TEST_ASSERT_EQUAL_STRING(buff2, "11111111""11111111""11111111""11100010");

	int num3 = -56;
	char buff3[32];
	IntToBinary(num3, buff3);
	TEST_ASSERT_EQUAL_STRING(buff3, "11111111""11111111""11111111""11001000");

}


void CountBits_test(void){
	TEST_ASSERT_EQUAL_INT(CountBits(4),1);
	TEST_ASSERT_EQUAL_INT(CountBits(5),2);
	TEST_ASSERT_EQUAL_INT(CountBits(7),3);
	TEST_ASSERT_EQUAL_INT(CountBits(15),4);
}

void ReplaceThirdByte_test(void){
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(255,255),0xFF00FF);
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(5,9),0x090005);
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(0xFFFFFF,0),0x00FFFF);
	TEST_ASSERT_EQUAL_INT(ReplaceThirdByte(0x910000,0x5B),0x5B0000);

}


int main(){
	UNITY_BEGIN();
	RUN_TEST(PowOfTwo_test);
	RUN_TEST(ReverseMasChar_test);
	RUN_TEST(IntToBinary_test_pos);
	RUN_TEST(IntToBinary_test_neg);
	RUN_TEST(CountBits_test);
	RUN_TEST(ReplaceThirdByte_test);
	return UNITY_END();

}
