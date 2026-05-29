#include "ServerFunc.h"

void PrintLogo(){
	printf("\e[1;34m   _____ _____ __ __ _____\e[0m\n");
	printf("\e[1;34m\\\\\\\\  __\\\\  __\\\\ \\\\ \\\\    \\\e[0m\n");
	printf("\e[1;34m\\\\\\\\\\  __\\\\ \\___\\  _ \\\\  \\ \\\e[0m\n");
	printf("\e[1;34m\\\\\\\\\\\\____\\\\____\\\\_\\\\_\\\\____\\server\e[0m\n");
}


int InitServer(struct Server* server, int port){
	server->fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (server->fd < 0) {
		printf("[X] Не удалось создать файловый дескрипор\n");
		return -1;
	}
	//printf("Удалось создать файловый дескриптор\n");

	server->addr.sin_family = AF_INET;
	server->addr.sin_addr.s_addr = INADDR_ANY;
	server->addr.sin_port = htons(port);
	if ( bind(server->fd, (struct sockaddr*)&(server->addr), sizeof(server->addr)) < 0) {
		printf("[X] Не удалось забиндить порт %d\n", port);
		close(server->fd);
		return -1;
	}

	PrintLogo();
	//printf("Был забинден порт %d\n", port);
	printf("[O] Сервер поднят\n");

	server->list_clients = InitList();
	return 0;
}

int GetMsg(struct Server* server, char* msg, struct sockaddr_in* addr){
	char buff[SIZE_BUFF];
	socklen_t len_addr = sizeof(*addr);
	int recv_recvfrom = recvfrom(server->fd, buff, sizeof(buff), 0, (struct sockaddr*)addr, &len_addr);
	if ( recv_recvfrom < 0) {
		int err = errno;
		printf("[X] Не удалось принять сообщение\n");
		printf("Error = %d | fd = %d\n", err, server->fd);
		return -1;
	}
	strcpy(msg, buff);
	return 1;
}

int SendMsg(struct Client* client, int fd_server, char* buff){
	char ans_buff[SIZE_BUFF + 10];
	printf("msg_count = %d\n", client->msg_count);
	sprintf(ans_buff, "%s %d", buff, client->msg_count);
	socklen_t len_client_addr = sizeof(client->addr);
	
	printf("addr = %d  | fd = %d | msg = %s\n", client->addr.sin_addr.s_addr, fd_server, ans_buff);
		
	ssize_t recv_sendto = sendto(fd_server, ans_buff, (strlen(ans_buff) + 1), 0, (struct sockaddr*)&(client->addr), len_client_addr);
	if ( recv_sendto == -1) {
		int err = errno;
		printf("[X] Не удалось отправить сообщеие \n");
		printf("Error = %d\n", err);
		return -1;
	}
	printf("отправлено\n");	
	return 1;

}

int IsItExitMsg(char* msg){
	if ( strcmp(msg, EXIT_WORD) == 0 ) {
		return 1;
	}else {
		return 0;
	}
}

void StartListeningMsg(struct Server* server){
	if (server == NULL) {
		printf("[X] Сервер равен NULL\n");
		return;
	}

	while(1) {
		char msg[SIZE_BUFF];		
		struct sockaddr_in client_addr;
		
		int recv_getmsg = GetMsg(server, msg, &client_addr);
		if (recv_getmsg == -1) {
			continue;
		}

		printf("Сервер принял сообщение от клиента: { %s }\n", msg);

		if ( IsItExitMsg(msg) ) {
			
			continue;
		}

		struct Client* search_client = SearchClient(server->list_clients, &client_addr);
		if ( search_client == NULL) {
			printf("Это оказался новый клиент\n");
			struct Client new_client;
			InitClient(&new_client, &client_addr);
			AddNode(server->list_clients, &new_client);
			SendMsg(&new_client, server->fd, msg);
		}else{
			printf("Это оказался старый клиент (msg_count = %d)\n", search_client->msg_count);
			search_client->msg_count++;
			SendMsg(search_client, server->fd, msg);
		}
	}
}
