#include "List.h"

struct List* InitList(int max_length){
	struct List* list = malloc(sizeof(struct List));
	list->length = 0;
	list->max_length = max_length;
	list->start = NULL;
}

void AddNode(struct List* list, char* msg){
	struct Node** cur_node = &(list->start);
	while(*cur_node != NULL){
		cur_node = &((*cur_node)->next);
	}
	*cur_node = malloc(sizeof(struct Node));
	strcpy((*cur_node)->msg, msg);
	(*cur_node)->next = NULL;
	list->length++;
}

void DeleteFirstMsg(struct List* list){
	if (list->start == NULL) {
		return;
	}

	struct Node* delete_node;
	delete_node = list->start;
	list->start = list->start->next;
	free(delete_node);
	list->length--;
	return;

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
