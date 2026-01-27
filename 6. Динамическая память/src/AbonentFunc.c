#include "AbonentFunc.h"

void PrintAbonent(struct Abonent* abo){
	if(abo == NULL){
		return;
	}
	printf("%s %s - %s\n", abo->name, abo->second_name, abo->tel);
}

void PrintListSince(struct ListNode* node){
	while(node != NULL){
		printf("%d. ", node->id);
		PrintAbonent(node->data);
		node = node->next;
	}
	printf("\n");
}

void PrintListInRange(struct ListNode *node, unsigned int range){
	for(int i=0; i<range; i++){
		if(node==NULL){
			break;
		}
		
		printf("%d. ", node->id);
		PrintAbonent(node->data);
		node = node->next;
	}
	printf("\n");
}

struct List* InitList(){
	struct List* list = malloc(sizeof(struct List));
	if(list==NULL){
		return NULL;
	}
	
	list->current_size = 0;	
	list->start = NULL;
	list->end = NULL;

	return list;
}

struct ListNode* InitStartOfList(struct Abonent* abonent){
	struct ListNode* node = malloc(sizeof(struct ListNode));
	if(node==NULL){
		return NULL;
	}
	
	node->id = START_ID;
	node->data = abonent;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

struct ListNode* InitNextNode(struct ListNode* cur_node, struct Abonent* abonent){
	if(cur_node == NULL){
		return NULL;
	}

	struct ListNode *next_node = malloc(sizeof(struct ListNode));
	
	next_node->id = cur_node->id + 1;
	next_node->data = abonent;
	next_node->prev = cur_node;
	next_node->next = NULL;

	cur_node->next = next_node;

	return next_node;
}

struct Abonent* InitAbonent(char* aname, char* asecond_name, char* atel){
	struct Abonent* abonent = malloc(sizeof(struct Abonent));
	if(abonent==NULL){
		return NULL;
	}
	
	strncpy(abonent->name, aname, SIZE_CHAR_ARRAY);
	strncpy(abonent->second_name, asecond_name, SIZE_CHAR_ARRAY);
	strncpy(abonent->tel, atel, SIZE_CHAR_ARRAY);
	
	return abonent;
}

int AddAbonent(struct List* list, struct Abonent* abonent){
	if(list == NULL){
		return 1;
	}

	if(list->start==NULL){
		list->current_size = 1;
		list->start = InitStartOfList(abonent);
		list->end = list->start;
		return 0;
	}
	
	list->current_size++;
	list->end = InitNextNode(list->end, abonent);
	
	return 0;
}

void FreeListNode(struct ListNode* node){
	free(node->data);
	free(node);
}

int DeleteAbonentId(struct List *list, unsigned int id){
	if(list == NULL){
		return 1;
	}
	
	int max_id = list->end->id; 
	if ( id > max_id || id < START_ID ){
		return 2;
	}
	
	struct ListNode* search_node = list->start;
	while(search_node->id != id){
		search_node = search_node->next;
	}

	if(search_node->prev!=NULL){
		search_node->prev->next = search_node->next;
	}else{
		list->start = list->start->next;
	}
	if(search_node->next!=NULL){
		search_node->next->prev = search_node->prev;
	}else{
		list->end = list->end->prev;
	}
	FreeListNode(search_node);
	list->current_size--;
	return 0;
}

struct Abonent* SearchAbonentName(struct List *list, char *aname){
	if(list == NULL){
		return NULL;
	}
	struct ListNode* search_node = list->start;
	while(search_node!=NULL){
		if(strcmp(search_node->data->name, aname) == 0){
			return search_node->data;
		}
		search_node = search_node->next;
	}
	return NULL;
}
