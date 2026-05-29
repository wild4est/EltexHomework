#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#include "RawIPV4.h"
#include "List.h"

#define SIZE_MSG_SEND 245
#define PORT 49134
#define IP_ADDR "127.0.0.1"
#define EXIT_CODE "exit"

struct Client{
	struct List* list_msg;
	int fd;
	struct sockaddr_in addr;
	int port;
	int port_server;
	uint32_t ip_addr;
	uint32_t ip_addr_server;
};

int InitClient(struct Client* client, int client_port, int port_server, const char* ip_addr, const char* ip_addr_server);
int SendMsg(struct Client* client, char* msg);
int GetMsg(struct Client* client);
int IsItExitMsg(char* msg);
