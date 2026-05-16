#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <mqueue.h>
#include "../../ServerStuff.h"

struct NodeMsg{
	char msg[SIZE_BUFF_MSG];
	struct NodeMsg* next;
};

struct ListMsg{
	int length;
	struct NodeMsg* start;
};

struct NodeClient{
	char name[SIZE_BUFF_NAME];
	mqd_t mqd_personal;
	struct NodeClient* next;
};

struct ListClient{
	int length;
	struct NodeClient* start;
};

struct ListMsg* InitListMsg();
void AddNodeMsg(struct ListMsg* list_msg, char* msg);
void DeleteNodeMsg(struct ListMsg* list_msg, char* msg);
void FreeListMsg(struct ListMsg* list_msg);

struct ListClient* InitListClient();
int ListContainsClient(struct ListClient* list_client, char* name);
void AddNodeClient(struct ListClient* list_client, char* name, mqd_t mqd);
void DeleteNodeClient(struct ListClient* list_client, char* name);
void FreeListClient(struct ListClient* list_client);
