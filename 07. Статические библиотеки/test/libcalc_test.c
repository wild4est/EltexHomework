#include "../src/libcalc/libcalc.h"
#include "../../Unity/unity.h"

void setUp(){
}

void tearDown(){
}

void Add_test(){
	int ans1;
	int ans2;
	int ans3;

	add(&ans1, 4, 10);
	add(&ans2, 4, -10);
	add(&ans3, -4, -10);

	TEST_ASSERT_EQUAL_INT(ans1, 14);
	TEST_ASSERT_EQUAL_INT(ans2, -6);
	TEST_ASSERT_EQUAL_INT(ans3, -14);
}

void Sub_test(){
	int ans1;
	int ans2;
	int ans3;

	sub(&ans1, 10, 4);
	sub(&ans2, 10, -4);
	sub(&ans3, -4, -10);

	TEST_ASSERT_EQUAL_INT(ans1, 6);
	TEST_ASSERT_EQUAL_INT(ans2, 14);
	TEST_ASSERT_EQUAL_INT(ans3, 6);
}

void Mul_test(){
	int ans1;
	int ans2;
	int ans3;

	mul(&ans1, 4, 10);
	mul(&ans2, 4, -3);
	mul(&ans3, -4, -5);

	TEST_ASSERT_EQUAL_INT(ans1, 40);
	TEST_ASSERT_EQUAL_INT(ans2, -12);
	TEST_ASSERT_EQUAL_INT(ans3, 20);
}

void Div_test(){
	int ans1;
	int ans2;
	int ans3;

	div(&ans1, 12, 4);
	div(&ans2, 20, -5);
	div(&ans3, -40, -4);

	TEST_ASSERT_EQUAL_INT(ans1, 3);
	TEST_ASSERT_EQUAL_INT(ans2, -4);
	TEST_ASSERT_EQUAL_INT(ans3, 10);

	TEST_ASSERT_EQUAL_INT(div(&ans1, 10, 0), 1);
}


int main(){
	UNITY_BEGIN();

	RUN_TEST(Add_test);
	RUN_TEST(Sub_test);
	RUN_TEST(Mul_test);
	RUN_TEST(Div_test);
	
	return UNITY_END();

}
