#include <stdio.h>
#include <malloc.h>
#include <string.h>
//#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#include "../../ServerStuff.h"

struct NodeMsg{
	char msg[SIZE_BUFF_MSG];
	struct NodeMsg* next;
};

struct ListMsg{
	int length;
	struct NodeMsg* start;
};

struct SharedMemory{
	int shm;
	void* ptr;
	sem_t* sem_read;
	sem_t* sem_write;
};

struct NodeClient{
	char name[SIZE_BUFF_NAME];
	//mqd_t mqd_personal;
	struct SharedMemory shared_mem_per;
	struct NodeClient* next;
};

struct ListClient{
	int length;
	struct NodeClient* start;
};

void InitSharedMemory(struct SharedMemory* shared_mem, int shm_fd, void* ptr, sem_t* sem_read, sem_t* sem_write);
void CloseSharedMemory(struct SharedMemory* shared_mem);
void UnlinkSharedMemory(char* name_shm, char* name_sem_rd, char* name_sem_wr);

struct ListMsg* InitListMsg();
void AddNodeMsg(struct ListMsg* list_msg, char* msg);
void DeleteNodeMsg(struct ListMsg* list_msg, char* msg);
void FreeListMsg(struct ListMsg* list_msg);

struct ListClient* InitListClient();
int ListContainsClient(struct ListClient* list_client, char* name);
void AddNodeClient(struct ListClient* list_client, char* name, struct SharedMemory shared_mem);
void DeleteNodeClient(struct ListClient* list_client, char* name);
void FreeListClient(struct ListClient* list_client);
