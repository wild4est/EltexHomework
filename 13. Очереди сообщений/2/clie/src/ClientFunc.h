#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <ncurses.h>
#include <unistd.h>
#include <errno.h> 
#include "List.h"
#include "../../ServerStuff.h"

#define CLIENT_NAME "Cow"
#define CLIENT_MSG "Moo"

#define MSG_COUNT 10

struct Client{
	char* name;
	mqd_t mqd_messaging;
	mqd_t mqd_personal;
	mqd_t mqd_exit;
	struct List* list_clients;
	struct List* list_msgs;
};

int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize, int oflag);
struct Client* InitClient(char* name);
int SendMsgToServer(mqd_t mqd_messaging, char* name, char* msg);
int GetMsgFromServer(mqd_t mqd_personal, struct List** list_msgs, struct List** list_clients);
int GetDataFromServer(struct Client* client);
//void* MessagingListener(void* args);
//void* BroadcastListener(void* args);
