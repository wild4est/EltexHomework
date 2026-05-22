#include "List.h"

struct List* InitList(){
	struct List* list = malloc(sizeof(struct List));
	if (list==NULL) {
		return NULL;
	}
	
	list->length=0;
	list->start = NULL;
	return list;
}

void AddNode(struct List* list, char* data){
	struct Node** cur_node = &(list->start);
	while(*cur_node != NULL){
		cur_node = &((*cur_node)->next);
	}
	*cur_node = malloc(sizeof(struct Node));
	strcpy((*cur_node)->data, data);
	(*cur_node)->next = NULL;
	list->length++;
}

void DeleteNode(struct List* list, char* data){
	if (list->start == NULL) {
		return;
	}
	
	struct Node* delete_node;
	if (strcmp(list->start->data, data) == 0) {
		delete_node = list->start;
		list->start = list->start->next;
		free(delete_node);
		list->length--;
		return;
	}	

	struct Node** cur_node = &(list->start);
	while(((*cur_node)->next != NULL) && (strcmp((*cur_node)->next->data, data) != 0)){
		(*cur_node) = (*cur_node)->next;
	}
	delete_node = (*cur_node)->next;
	(*cur_node)->next = (*cur_node)->next->next;
	free(delete_node);
	list->length--;
}

void FreeList(struct List* list){
	struct Node* cur_node = list->start;
	while(cur_node != NULL){
		struct Node* delete_node = cur_node;
		cur_node = cur_node->next;
		free(delete_node);
	}
	free(list);
}
