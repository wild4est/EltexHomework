#include "../src/AbonentFunc.h"
#include "../../Unity/unity.h"

void setUp(){
}


void tearDown(){

}

void InitList_test(){
	struct List* list = InitList();
	TEST_ASSERT_EQUAL_PTR(list->start, NULL);
	TEST_ASSERT_EQUAL_PTR(list->end, NULL);
	TEST_ASSERT_EQUAL_INT(list->current_size, 0);
}

void InitAbonent_test(){
	char aname[] = "Name";
	char asecond_name[] = "Secname";
	char atel[] = "123";

	struct Abonent* abo = InitAbonent(aname, asecond_name, atel);
	TEST_ASSERT_EQUAL_STRING(abo->name, aname);
	TEST_ASSERT_EQUAL_STRING(abo->second_name, asecond_name);
	TEST_ASSERT_EQUAL_STRING(abo->tel, atel);
}

void InitAbonent_test_correct_data(){
	char aname[] = "Namenamenamenamename";
	char asecond_name[] = "Secnamesecnamesecname";
	char atel[] = "12345678901234567890";

	struct Abonent* abo = InitAbonent(aname, asecond_name, atel);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(abo->name, aname, SIZE_CHAR_ARRAY);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(abo->second_name, asecond_name, SIZE_CHAR_ARRAY);
	TEST_ASSERT_EQUAL_CHAR_ARRAY(abo->tel, atel, SIZE_CHAR_ARRAY);
}

void InitStartOfList_test(){
	struct Abonent* abo = InitAbonent("Name", "Secname", "123");
	struct ListNode* node = InitStartOfList(abo);
	TEST_ASSERT_EQUAL_INT(node->id, START_ID);
	TEST_ASSERT_EQUAL_PTR(node->data, abo);
	TEST_ASSERT_EQUAL_PTR(node->prev, NULL);
	TEST_ASSERT_EQUAL_PTR(node->next, NULL);
}

void InitNextNode_test(){
	struct Abonent* abo = InitAbonent("Name", "Secname", "123");
	struct ListNode* start_of_list = InitStartOfList(abo);
	struct ListNode* node = InitNextNode(start_of_list, abo);
	TEST_ASSERT_EQUAL_INT(node->id, START_ID+1);
	TEST_ASSERT_EQUAL_PTR(node->data, abo);
	TEST_ASSERT_EQUAL_PTR(node->prev, start_of_list);
	TEST_ASSERT_EQUAL_PTR(node->next, NULL);
}

void AddAbonent_test(){
	
	char aname1[] = "Name1";
	char asecond_name1[] = "Secname1";
	char atel1[] = "1231";

	char aname2[] = "Name2";
	char asecond_name2[] = "Secname2";
	char atel2[] = "1232";

	struct Abonent* abo1 = InitAbonent(aname1, asecond_name1, atel1);
	struct Abonent* abo2 = InitAbonent(aname2, asecond_name2, atel2);

	struct List* list = InitList();
	TEST_ASSERT_EQUAL_INT(AddAbonent(NULL, abo1), 1);
	TEST_ASSERT_EQUAL_INT(AddAbonent(list, abo1), 0);
	TEST_ASSERT_EQUAL_INT(AddAbonent(list, abo2), 0);
}


void SearchAbonentName_test(){
	struct List* list = InitList();
	struct Abonent* abo1 = InitAbonent("Maelle", "Dessendre", "456789");
	struct Abonent* abo2 = InitAbonent("Verso", "Dessendre", "12345");
	struct Abonent* abo3 = InitAbonent("Lune", "Moon", "23456");
	
	AddAbonent(list, abo1);
	AddAbonent(list, abo2);
	AddAbonent(list, abo3);

	TEST_ASSERT_EQUAL_PTR(SearchAbonentName(list, "Verso"), abo2);
	TEST_ASSERT_EQUAL_PTR(SearchAbonentName(list, "Alicia"), NULL);
	TEST_ASSERT_EQUAL_PTR(SearchAbonentName(NULL, "Alicia"), NULL);
}

void DeleteAbonentId_test(){
	struct List* list = InitList();
	struct Abonent* abo1 = InitAbonent("Maelle", "Dessendre", "456789");
	struct Abonent* abo2 = InitAbonent("Gustave", "Dessendre", "12345");
	struct Abonent* abo3 = InitAbonent("Lune", "Moon", "23456");
	
	AddAbonent(list, abo1);
	AddAbonent(list, abo2);
	AddAbonent(list, abo3);

	int result = DeleteAbonentId(list, 1);
	TEST_ASSERT_EQUAL_INT(result, 0);
	struct Abonent* delete_abonent = SearchAbonentName(list, "Gustave");
	TEST_ASSERT_EQUAL_PTR(delete_abonent, NULL);
	
	TEST_ASSERT_EQUAL_INT(DeleteAbonentId(list, 10), 2);
	TEST_ASSERT_EQUAL_INT(DeleteAbonentId(NULL, 0), 1);
}


int main(){
	UNITY_BEGIN();
	RUN_TEST(InitList_test);
	RUN_TEST(InitAbonent_test);
	RUN_TEST(InitAbonent_test_correct_data);
	RUN_TEST(InitStartOfList_test);
	RUN_TEST(InitNextNode_test);
	RUN_TEST(AddAbonent_test);
	RUN_TEST(SearchAbonentName_test);
	RUN_TEST(DeleteAbonentId_test);

	return UNITY_END();
}
