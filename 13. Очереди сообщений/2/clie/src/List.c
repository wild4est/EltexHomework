#include "List.h"

struct List* InitList(){
	struct List* list_msg = malloc(sizeof(struct List));
	if (list_msg==NULL) {
		return NULL;
	}
	
	list_msg->length=0;
	list_msg->start = NULL;
	return list_msg;
}

void AddNode(struct List* list_msg, char* msg){
	struct Node** cur_node = &(list_msg->start);
	while(*cur_node != NULL){
		cur_node = &((*cur_node)->next);
	}
	*cur_node = malloc(sizeof(struct Node));
	strcpy((*cur_node)->msg, msg);
	(*cur_node)->next = NULL;
	list_msg->length++;
}

void DeleteNode(struct List* list_msg, char* msg){
	if (list_msg->start == NULL) {
		return;
	}
	
	struct Node* delete_node;
	if (strcmp(list_msg->start->msg, msg) == 0) {
		delete_node = list_msg->start;
		list_msg->start = list_msg->start->next;
		free(delete_node);
		list_msg->length--;
		return;
	}	

	struct Node** cur_node = &(list_msg->start);
	while(((*cur_node)->next != NULL) && (strcmp((*cur_node)->next->msg, msg) != 0)){
		(*cur_node) = (*cur_node)->next;
	}
	delete_node = (*cur_node)->next;
	(*cur_node)->next = (*cur_node)->next->next;
	free(delete_node);
	list_msg->length--;
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
