#include "unity.h"
#include "../src/BinaryFunc.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void powOfTwo_test(void){
	TEST_ASSERT_EQUAL_INT(powOfTwo(1),1);
	TEST_ASSERT_EQUAL_INT(powOfTwo(2),2);
	TEST_ASSERT_EQUAL_INT(powOfTwo(5),3);
	TEST_ASSERT_EQUAL_INT(powOfTwo(9),4);
}

void reverseMasChar_test(void){
	char buff1[] = "abcde";
	char rev_buff1[5];
	reverseMasChar(buff1, rev_buff1, 5);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(rev_buff1, "edcba", 5);

	char buff2[] = "hello world";
	char rev_buff2[11];
	reverseMasChar(buff2, rev_buff2, 11);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(rev_buff2, "dlrow olleh", 11);

	char buff3[] = "123";
	char rev_buff3[3];
	reverseMasChar(buff3, rev_buff3, 3);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(rev_buff3, "321", 3);
}


void toBinary_test_pos(void){
	int num1 = 10;
	char buff1[32];
	toBinary(num1, buff1);
	TEST_ASSERT_EQUAL_STRING(buff1, "1010");
	
	int num2 = 30;
	char buff2[32];
	toBinary(num2, buff2);
	TEST_ASSERT_EQUAL_STRING(buff2, "11110");

	int num3 = 56;
	char buff3[32];
	toBinary(num3, buff3);
	TEST_ASSERT_EQUAL_STRING(buff3, "111000");

}

void toBinary_test_neg(void){
	int num1 = -10;
	char buff1[32];
	toBinary(num1, buff1);
	TEST_ASSERT_EQUAL_STRING(buff1, "11111111""11111111""11111111""11110110");
	
	int num2 = -30;
	char buff2[32];
	toBinary(num2, buff2);
	TEST_ASSERT_EQUAL_STRING(buff2, "11111111""11111111""11111111""11100010");

	int num3 = -56;
	char buff3[32];
	toBinary(num3, buff3);
	TEST_ASSERT_EQUAL_STRING(buff3, "11111111""11111111""11111111""11001000");

}


void countBits_test(void){
	TEST_ASSERT_EQUAL_INT(countBits(4),1);
	TEST_ASSERT_EQUAL_INT(countBits(5),2);
	TEST_ASSERT_EQUAL_INT(countBits(7),3);
	TEST_ASSERT_EQUAL_INT(countBits(15),4);
}

void replaceThirdByte_test(void){
	TEST_ASSERT_EQUAL_INT(replaceThirdByte(255,255),0xFF00FF);
	TEST_ASSERT_EQUAL_INT(replaceThirdByte(5,9),0x090005);
	TEST_ASSERT_EQUAL_INT(replaceThirdByte(0xFFFFFF,0),0x00FFFF);
	TEST_ASSERT_EQUAL_INT(replaceThirdByte(0x910000,0x5B),0x5B0000);

}


int main(){
	UNITY_BEGIN();
	RUN_TEST(powOfTwo_test);
	RUN_TEST(reverseMasChar_test);
	RUN_TEST(toBinary_test_pos);
	RUN_TEST(toBinary_test_neg);
	RUN_TEST(countBits_test);
	RUN_TEST(replaceThirdByte_test);
	return UNITY_END();

}
