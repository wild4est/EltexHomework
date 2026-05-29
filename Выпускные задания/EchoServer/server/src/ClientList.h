#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

struct Client{
	struct sockaddr_in addr;
	int msg_count;
};

struct Node{
	struct Client client;
	struct Node* next;
};

struct List{
	int length;
	struct Node* start;
};

struct List* InitList();
void InitClient(struct Client* client, struct sockaddr_in* addr);
void AddNode(struct List* list, struct Client* client);
struct Client* SearchClient(struct List* list, struct sockaddr_in* new_addr);

//void DeleteNodeByFd(struct List* list, int fd);
//void DeleteNodeById(struct List* list, int id);
