#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

#include "ClientList.h"

#define EXIT_WORD "exit"
#define SIZE_BUFF 245

struct Server{
	struct List* list_clients;
	struct sockaddr_in addr;	
	int fd;
};

void PrintLogo();
int GetMsg(struct Server* server, char* msg, struct sockaddr_in* addr);
int InitServer(struct Server* server, int port);
void StartListeningMsg(struct Server* server);
