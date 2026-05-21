#include "ClientFunc.h"

int InitShm(int* shm_fd, void** ptr, const char* name, int oflag, size_t size){
        *shm_fd = shm_open(name, oflag, 0666);
        if (*shm_fd == -1) {
		int err = errno;
		printf("Error[ InitShm ]= %d\n", err);
	}
	
	int recv = ftruncate(*shm_fd, size);
        if (recv == -1) {
		int err = errno;
		printf("Error[ InitShm ]= %d\n", err);
	}
	
	*ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, *shm_fd, 0);
        if (*ptr == (void*)-1) {
		int err = errno;
		printf("Error[ InitShm ]= %d\n", err);
	}

	return 0;
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


struct Client* InitClient(char* name){
	struct Client* client = malloc(sizeof(struct Client));
	client->name = name;
	
	char name_for_shm[SIZE_BUFF_NAME+1];
   	sprintf(name_for_shm, "/%s", name);
	
	InitShm(&(client->shared_mem_messaging.shm), &(client->shared_mem_messaging.ptr), SHM_MESSAGING, O_RDWR, SIZE_BUFF_MSG);
	InitShm(&(client->shared_mem_personal.shm), &(client->shared_mem_personal.ptr), name_for_shm, O_CREAT | O_RDWR, SIZE_BUFF_MSG);
	InitShm(&(client->shared_mem_exit.shm), &(client->shared_mem_exit.ptr), SHM_EXIT, O_RDWR, SIZE_BUFF_MSG);

	char name_for_sem[SIZE_BUFF_NAME+4];
	sprintf(name_for_sem, "/rd_%s", name);
	client->shared_mem_messaging.sem_read = sem_open(SEM_MESSAGING_RD, 0);

	client->shared_mem_personal.sem_read = sem_open(name_for_sem, O_CREAT, 0666, 0);
	client->shared_mem_exit.sem_read = sem_open(SEM_EXIT_RD, 0);

	sprintf(name_for_sem, "/wr_%s", name);	
	client->shared_mem_messaging.sem_write = sem_open(SEM_MESSAGING_WR, 0);
	client->shared_mem_personal.sem_write = sem_open(name_for_sem, O_CREAT, 0666, 1);
	client->shared_mem_exit.sem_write = sem_open(SEM_EXIT_WR, 0);

	client->list_clients = InitList();
	client->list_msgs = InitList();

	return client;
}


int GetDataFromServer(struct Client* client){
	ssize_t recv;
	char get_name[SIZE_BUFF_MSG];
	sem_wait(client->shared_mem_personal.sem_read);
	strcpy(get_name, client->shared_mem_personal.ptr);
	sem_post(client->shared_mem_personal.sem_write);
	
	while(strcmp(get_name, ANSWER_END) != 0){
		AddNode(client->list_clients, get_name);
		
		sem_wait(client->shared_mem_personal.sem_read);
		strcpy(get_name, client->shared_mem_personal.ptr);
		sem_post(client->shared_mem_personal.sem_write);
		
	}

	char get_msg[SIZE_BUFF_MSG];
	sem_wait(client->shared_mem_personal.sem_read);
	strcpy(get_msg, client->shared_mem_personal.ptr);
	sem_post(client->shared_mem_personal.sem_write);
	
	while(strcmp(get_msg, ANSWER_END) != 0){
		AddNode(client->list_msgs, get_msg);
	
		sem_wait(client->shared_mem_personal.sem_read);
		strcpy(get_msg, client->shared_mem_personal.ptr);
		sem_post(client->shared_mem_personal.sem_write);

	}

	return 0;
}

int SendMsgToServer(struct SharedMemory* shared_mem, char* name, char* text_msg){
	sem_wait(shared_mem->sem_write);
	sprintf(shared_mem->ptr, "%s: %s", name, text_msg);
	sem_post(shared_mem->sem_read);

	return 0;
}

int GetMsgFromServer(struct SharedMemory* shared_mem, struct List** list_msgs, struct List** list_clients){
	char msg[SIZE_BUFF_MSG];
	sem_wait(shared_mem->sem_read);
	strcpy(msg, shared_mem->ptr);
	sem_post(shared_mem->sem_write);

	if (msg[0] == '+') {
		memmove(msg, msg+1, strlen(msg));
		AddNode(*list_clients, msg);
	}else if(msg[0] == '-') {
		memmove(msg, msg+1, strlen(msg));
		DeleteNode(*list_clients, msg);
	}else{
		if ((*list_msgs)->length >= MSG_COUNT) {
			DeleteNode(*list_msgs, (*list_msgs)->start->msg);
		}
		AddNode(*list_msgs, msg);
	}

	return 0;	
}

