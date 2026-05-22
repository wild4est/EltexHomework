#include "../src/AbonentFunc.h"
#include "../../Unity/unity.h"

struct vector vec;

void setUp(){
	InitVector(&vec);
}


void tearDown(){

}

void InitVector_test(){
	TEST_ASSERT_EQUAL_INT(vec.capacity, SIZE_ABONENTS);
	TEST_ASSERT_EQUAL_INT(vec.current_size, 0);
}

void AddAbonent_test(){
	TEST_ASSERT_EQUAL_INT(AddAbonent(NULL, "Name", "Secname", "123"), 1);
	TEST_ASSERT_EQUAL_INT(AddAbonent(&vec, "Name", "Secname", "123"), 0);
}

void AddAbonent_test_capacity(){
	for(int i=0; i<SIZE_ABONENTS; i++){
		AddAbonent(&vec, "Name", "Secname", "123");
	}
	TEST_ASSERT_EQUAL_INT(AddAbonent(&vec, "Name", "Secname", "123"), 1);
}

void AddAbonent_test_correct_data(){
	char aname[] = "Namenamenamenamename";
	char asname[] = "Secnamesecnamesecname";
	char atel[] = "12345678901234567890";
	AddAbonent(&vec, aname, asname, atel);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(vec.abonents[0].name, aname , SIZE_CHAR_ARRAY);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(vec.abonents[0].second_name, asname , SIZE_CHAR_ARRAY);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(vec.abonents[0].tel, atel , SIZE_CHAR_ARRAY);
}

void SearchAbonentName_test(){
	AddAbonent(&vec, "Maelle", "Dessendre", "456789");
	AddAbonent(&vec, "Verso", "Dessendre", "12345");
	AddAbonent(&vec, "Lune", "Moon", "23456");

	TEST_ASSERT_EQUAL_PTR(SearchAbonentName(&vec, "Verso"), &vec.abonents[1]);
	TEST_ASSERT_EQUAL_PTR(SearchAbonentName(&vec, "Alicia"), NULL);
	TEST_ASSERT_EQUAL_PTR(SearchAbonentName(NULL, "Alicia"), NULL);
}

void DeleteAbonentId_test(){
	AddAbonent(&vec, "Maelle", "Dessendre", "456789");
	AddAbonent(&vec, "Gustave", "Gray", "12345");
	AddAbonent(&vec, "Lune", "Moon", "23456");

	int result = DeleteAbonentId(&vec, 1);
	TEST_ASSERT_EQUAL_INT(result, 0);
	struct abonent* delete_abonent = SearchAbonentName(&vec, "Gustave");
	TEST_ASSERT_EQUAL_PTR(delete_abonent, NULL);
	
	TEST_ASSERT_EQUAL_INT(DeleteAbonentId(&vec, 10), 1);
	TEST_ASSERT_EQUAL_INT(DeleteAbonentId(NULL, 0), 1);
}


int main(){
	printf("В процессе тестирования выводятся сообщения об ошибках.\n");
	printf("Это нормально, так как они выводятся в процессе тестирования этих ошибок.\n");
	printf("-----------------------------\n");

	UNITY_BEGIN();
	RUN_TEST(InitVector_test);
	RUN_TEST(AddAbonent_test);
	RUN_TEST(AddAbonent_test_capacity);
	RUN_TEST(AddAbonent_test_correct_data);
	RUN_TEST(SearchAbonentName_test);
	RUN_TEST(DeleteAbonentId_test);

	return UNITY_END();
}
