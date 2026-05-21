#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
//#include <mqueue.h>
#include <ncurses.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "List.h"
#include "../../ServerStuff.h"

#define CLIENT_NAME "Cow"
#define CLIENT_MSG "Moo"

#define MSG_COUNT 10

struct SharedMemory{
        int shm;
        void* ptr;
        sem_t* sem_read;
        sem_t* sem_write;
};

struct Client{
	char* name;
	//mqd_t mqd_messaging;
	//mqd_t mqd_personal;
	//mqd_t mqd_exit;
	
	/*sem_t* sem_messaging;
        sem_t* sem_personal;
        sem_t* sem_exit;

        int shm_messaging;
        int shm_personal;
        int shm_exit;

        void* ptr_messaging;
        void* ptr_personal;
        void* ptr_exit;
	*/

	struct SharedMemory shared_mem_messaging;
	struct SharedMemory shared_mem_personal;
	struct SharedMemory shared_mem_exit;

	struct List* list_clients;
	struct List* list_msgs;
};

//int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize, int oflag);
int InitShm(int* shm_fd, void** ptr, const char* name,int oflag, size_t size);
void CloseSharedMemory(struct SharedMemory* shared_mem);
void UnlinkSharedMemory(char* name_shm, char* name_sem_rd, char* name_sem_wr);

struct Client* InitClient(char* name);
int SendMsgToServer(struct SharedMemory* shared_mem, char* name, char* msg);
int GetMsgFromServer(struct SharedMemory* shared_mem, struct List** list_msgs, struct List** list_clients);
int GetDataFromServer(struct Client* client);
//void* MessagingListener(void* args);
//void* BroadcastListener(void* args);
