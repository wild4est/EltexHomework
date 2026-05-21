#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

//#include <mqueue.h>

#include "List.h"
#include "../../ServerStuff.h"

#define MAX_COUNT_CLIENTS 5
#define MAX_COUNT_MSGS 10

struct Server{
        struct ListClient* list_clients;
        struct ListMsg* list_msgs;

	struct SharedMemory shared_mem_new_client;
	struct SharedMemory shared_mem_messaging;
	struct SharedMemory shared_mem_exit;

	/*sem_t* sem_new_clients;
	sem_t* sem_messaging;
	sem_t* sem_exit;

	int shm_new_clients;
	int shm_messaging;
	int shm_exit;

	void* ptr_new_clients;
	void* ptr_messaging;
	void* ptr_exit;*/

	//mqd_t mqd_new_clients;
        //mqd_t mqd_messaging;
        //mqd_t mqd_broadcast;
        //mqd_t mqd_exit;
};

//int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize, int oflag);
int InitShm(int* shm_fd, void** ptr, const char* name, size_t size);
struct Server* InitServer();
int OpenQueueToClient(char* name, struct SharedMemory* shared_mem);
void SendMsgToAllClients(struct ListClient* list, char* answer);
void SendMsgToClient(struct SharedMemory* shared_mem, char* answer);
void* NewClientListener(void* args);
void* MessagingListener(void* args);
void* ExitListener(void* args);
