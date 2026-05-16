#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <mqueue.h>
#include "../../ServerStuff.h"

struct Node{
	char msg[SIZE_BUFF_MSG];
	struct Node* next;
};

struct List{
	int length;
	struct Node* start;
};

struct List* InitList();
void AddNode(struct List* list_msg, char* msg);
void DeleteNode(struct List* list_msg, char* msg);
void FreeList(struct List* list);
