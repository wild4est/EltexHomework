#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define MAX_
#define SIZE_BUFF 255

struct Node{
	char msg[SIZE_BUFF];
	struct Node* next;
};

struct List{
	int length;
	int max_length;
	struct Node* start;
};

struct List* InitList(int max_length);
void AddNode(struct List* list, char* msg);
void DeleteFirstMsg(struct List* list);

//void DeleteNodeById(struct List* list, int id);
