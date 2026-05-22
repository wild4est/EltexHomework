#include "List.h"

struct ListMsg* InitListMsg(){
	struct ListMsg* list_msg = malloc(sizeof(struct ListMsg));
	if (list_msg==NULL) {
		return NULL;
	}
	
	list_msg->length=0;
	list_msg->start = NULL;
	return list_msg;
}

void AddNodeMsg(struct ListMsg* list_msg, char* msg){
	struct NodeMsg** cur_node = &(list_msg->start);
	while(*cur_node != NULL){
		cur_node = &((*cur_node)->next);
	}
	*cur_node = malloc(sizeof(struct NodeMsg));
	strcpy((*cur_node)->msg, msg);
	(*cur_node)->next = NULL;
	list_msg->length++;
}

void DeleteNodeMsg(struct ListMsg* list_msg, char* msg){
	if (list_msg->start == NULL) {
		return;
	}
	
	struct NodeMsg* delete_node;
	if (strcmp(list_msg->start->msg, msg) == 0) {
		delete_node = list_msg->start;
		list_msg->start = list_msg->start->next;
		free(delete_node);
		list_msg->length--;
		return;
	}	

	struct NodeMsg** cur_node = &(list_msg->start);
	while(((*cur_node)->next != NULL) && (strcmp((*cur_node)->next->msg, msg) != 0)){
		(*cur_node) = (*cur_node)->next;
	}
	delete_node = (*cur_node)->next;
	(*cur_node)->next = (*cur_node)->next->next;
	free(delete_node);
	list_msg->length--;
}

void FreeListMsg(struct ListMsg* list_msg){
        struct NodeMsg* cur_node = list_msg->start;
        while(cur_node != NULL){
                struct NodeMsg* delete_node = cur_node;
                cur_node = cur_node->next;
                free(delete_node);
        }
        free(list_msg);
}

struct ListClient* InitListClient(){
	struct ListClient* list_client = malloc(sizeof(struct ListClient));
	if (list_client==NULL) {
		return NULL;
	}
	
	list_client->length=0;
	list_client->start = NULL;
	return list_client;
}

void AddNodeClient(struct ListClient* list_client, char* name, mqd_t mqd){
	struct NodeClient** cur_node = &(list_client->start);
	while(*cur_node != NULL){
		cur_node = &((*cur_node)->next);
	}
	*cur_node = malloc(sizeof(struct NodeClient));
	strcpy((*cur_node)->name, name);
	(*cur_node)->mqd_personal = mqd;
	(*cur_node)->next = NULL;
	list_client->length++;
}

void DeleteNodeClient(struct ListClient* list_client, char* name){
	if (list_client->start == NULL) {
		return;
	}
	
	struct NodeClient* delete_node;
	if (strcmp(list_client->start->name, name) == 0) {
		delete_node = list_client->start;
		list_client->start = list_client->start->next;
		free(delete_node);
		list_client->length--;
		return;
	}	

	struct NodeClient** cur_node = &(list_client->start);
	while(((*cur_node)->next != NULL) && (strcmp((*cur_node)->next->name, name) != 0)){
		(*cur_node) = (*cur_node)->next;
	}
	delete_node = (*cur_node)->next;
	(*cur_node)->next = (*cur_node)->next->next;
	free(delete_node);
	list_client->length--;
}

void FreeListClient(struct ListClient* list_client){
        struct NodeClient* cur_node = list_client->start;
        while(cur_node != NULL){
                struct NodeClient* delete_node = cur_node;
                cur_node = cur_node->next;
                free(delete_node);
        }
        free(list_client);
}  
