#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>
#include "List.h"
#include "../../ServerStuff.h"

#define MAX_COUNT_CLIENTS 5
#define MAX_COUNT_MSGS 10

struct Server{
        struct ListClient* list_clients;
        struct ListMsg* list_msgs;
        mqd_t mqd_new_clients;
        mqd_t mqd_messaging;
        mqd_t mqd_broadcast;
        mqd_t mqd_exit;
};

int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize, int oflag);
struct Server* InitServer();
int OpenQueueToClient(char* name, mqd_t* mqd_client);
void SendMsgToAllClients(struct ListClient* list, char* answer);
void SendMsgToClient(mqd_t mqd_client, char* answer);
void* NewClientListener(void* args);
void* MessagingListener(void* args);
void* ExitListener(void* args);
