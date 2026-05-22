#include "List.h"

void InitSharedMemory(struct SharedMemory* shared_mem, int shm_fd, void* ptr, sem_t* sem_read, sem_t* sem_write){
	shared_mem->shm = shm_fd;
	shared_mem->ptr = ptr;
	shared_mem->sem_read = sem_read;
	shared_mem->sem_write = sem_write;
}

void CloseSharedMemory(struct SharedMemory* shared_mem){
        munmap(shared_mem->ptr, SIZE_BUFF_MSG);
        close(shared_mem->shm);
  
        sem_close(shared_mem->sem_read);
        sem_close(shared_mem->sem_write);
}


void UnlinkSharedMemory(char* name_shm, char* name_sem_rd, char* name_sem_wr){
	unlink(name_shm);
	sem_unlink(name_sem_rd);
        sem_unlink(name_sem_wr);
}


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

void AddNodeClient(struct ListClient* list_client, char* name, struct SharedMemory shared_mem){
	struct NodeClient** cur_node = &(list_client->start);
	while(*cur_node != NULL){
		cur_node = &((*cur_node)->next);
	}
	*cur_node = malloc(sizeof(struct NodeClient));
	strcpy((*cur_node)->name, name);
	(*cur_node)->shared_mem_per = shared_mem;
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
	
	char name_shm[SIZE_BUFF_NAME+1];
        sprintf(name_shm, "/%s", (*cur_node)->name);
	char name_sem_rd[SIZE_BUFF_NAME+4];
   	sprintf(name_sem_rd, "/rd_%s", (*cur_node)->name);
       	char name_sem_wr[SIZE_BUFF_NAME+4];
        sprintf(name_sem_wr, "/wr_%s", (*cur_node)->name);
	
	CloseSharedMemory(&((*cur_node)->shared_mem_per));
	//UnlinkSharedMemory(name_shm, name_sem_rd, name_sem_wr);

	free(delete_node);
	list_client->length--;
}

void FreeListClient(struct ListClient* list_client){
        struct NodeClient* cur_node = list_client->start;
        while(cur_node != NULL){
                struct NodeClient* delete_node = cur_node;
                cur_node = cur_node->next;
	
		char name_shm[SIZE_BUFF_NAME+1];
	        sprintf(name_shm, "/%s", cur_node->name);
        	char name_sem_rd[SIZE_BUFF_NAME+4];
   		sprintf(name_sem_rd, "/rd_%s", cur_node->name);
       		char name_sem_wr[SIZE_BUFF_NAME+4];
        	sprintf(name_sem_wr, "/wr_%s", cur_node->name);

		CloseSharedMemory(&(cur_node->shared_mem_per));  
		//UnlinkSharedMemory(name_shm, name_sem_rd, name_sem_wr);
		free(delete_node);
        }
        free(list_client);
}  
