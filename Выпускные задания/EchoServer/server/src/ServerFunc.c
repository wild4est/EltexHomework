#include "ServerFunc.h"

void PrintLogo(){
	printf("\e[1;34m   _____ _____ __ __ _____\e[0m\n");
	printf("\e[1;34m\\\\\\\\  __\\\\  __\\\\ \\\\ \\\\    \\\e[0m\n");
	printf("\e[1;34m\\\\\\\\\\  __\\\\ \\___\\  _ \\\\  \\ \\\e[0m\n");
	printf("\e[1;34m\\\\\\\\\\\\____\\\\____\\\\_\\\\_\\\\____\\server\e[0m\n");
	printf("Для выхода нажмите CTR + C\n\n");
}


int InitServer(struct Server* server, int port, const char* ip_addr){
	server->fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (server->fd < 0) {
		printf("[X] Не удалось создать файловый дескрипор\n");
		return -1;
	}
	int opt = 1;
        setsockopt(server->fd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));

	server->addr.sin_family = AF_INET;
	server->addr.sin_addr.s_addr = INADDR_ANY;
	server->addr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip_addr, &(server->ip_addr)) <= 0) {
                printf("[X] Указан некорректный адрес сервера\n");
                close(server->fd);
                return -1;
        }

	PrintLogo();
	printf("[O] Сервер поднят\n");

	server->list_clients = InitList();
	return 0;
}

void FreeServer(struct Server* server){
	struct Node* cur_node = server->list_clients->start;
        while(cur_node != NULL) {
                SendCode(&(cur_node->client), server->fd, server->addr.sin_port, server->ip_addr, EXIT_CODE);
                printf("[!] port = %d | addr = %X\n", ntohs(cur_node->client.addr.sin_port), cur_node->client.addr.sin_addr.s_addr );
		cur_node = cur_node->next;
        }
        FreeList(server->list_clients);
        //close(server->fd);
}

int GetMsg(struct Server* server, char* msg, struct sockaddr_in* addr){
	while (1) {
		char buff[SIZE_BUFF];
		int recv_recvfrom = recvfrom(server->fd, buff, sizeof(buff), 0, NULL, NULL);
		if ( recv_recvfrom < 0) {
			int err = errno;
			printf("[X] Не удалось принять сообщение\n");
			printf("Error = %d | fd = %d\n", err, server->fd);
			return -1;
		}
		
		struct Packet_IPV4* packet_ipv4 = (struct Packet_IPV4*)(buff);
		if ( PacketIPV4Check(packet_ipv4, server->ip_addr,server->addr.sin_port) ) {
			strcpy(msg, packet_ipv4->packet_udp.data);
			addr->sin_family = AF_INET;
			addr->sin_addr.s_addr = packet_ipv4->source_address;	
			addr->sin_port = packet_ipv4->packet_udp.source_port;
			printf("От пользователя[ addr = %X | port = %d] пришло сообщение:\n", addr->sin_addr.s_addr, ntohs(addr->sin_port));
			printf("{ %s }\n", packet_ipv4->packet_udp.data);
			break;
		}
	}
	
	return 1;
}

int SendCode(struct Client* client, int fd_server, uint16_t server_port, uint32_t server_ip_addr, char* code){
	socklen_t len_client_addr = sizeof(client->addr);
	
	struct Packet_UDP packet_udp;
	FillPacketUDP(&packet_udp, server_port, client->addr.sin_port, code);
	struct Packet_IPV4 packet_ipv4;
	FillPacketIPV4(&packet_ipv4, server_ip_addr, client->addr.sin_addr.s_addr, &packet_udp);

	ssize_t recv_sendto = sendto(fd_server, (char *)&packet_ipv4, sizeof(packet_ipv4), 0, (struct sockaddr*)&(client->addr), len_client_addr);
	if ( recv_sendto == -1) {
		int err = errno;
		printf("[X] Не удалось отправить сообщеие \n");
		printf("Error = %d\n", err);
		return -1;
	}
	printf("Пользователь[ addr = %X | port = %d] оповещён:\n", client->addr.sin_addr.s_addr, ntohs(client->addr.sin_port));
	printf("{ %s }\n", code);
	return 1;

}

int SendMsg(struct Client* client, int fd_server, uint16_t server_port, uint32_t server_ip_addr, char* buff){
	char ans_buff[SIZE_BUFF + 10];
	//printf("msg_count = %d\n", client->msg_count);
	sprintf(ans_buff, "%s %d", buff, client->msg_count);
	socklen_t len_client_addr = sizeof(client->addr);
	
	struct Packet_UDP packet_udp;
	FillPacketUDP(&packet_udp, server_port, client->addr.sin_port, ans_buff);
	struct Packet_IPV4 packet_ipv4;
	FillPacketIPV4(&packet_ipv4, server_ip_addr, client->addr.sin_addr.s_addr, &packet_udp);

	ssize_t recv_sendto = sendto(fd_server, (char *)&packet_ipv4, sizeof(packet_ipv4), 0, (struct sockaddr*)&(client->addr), len_client_addr);
	if ( recv_sendto == -1) {
		int err = errno;
		printf("[X] Не удалось отправить сообщеие \n");
		printf("Error = %d\n", err);
		return -1;
	}
	printf("Пользователю[ addr = %X | port = %d] отправлено сообщение:\n", client->addr.sin_addr.s_addr, ntohs(client->addr.sin_port));
	printf("{ %s }\n", ans_buff);
	return 1;

}

int IsItExitMsg(char* msg){
	if ( strcmp(msg, EXIT_CODE) == 0 ) {
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
		
		if ( IsItExitMsg(msg) ) {
			DeleteNodeByAddr(server->list_clients, &client_addr);	
			printf("Пользователь[ addr = %X | port = %d ] покинул сервер\n", client_addr.sin_addr.s_addr, ntohs(client_addr.sin_port));
			continue;
		}

		struct Client* search_client = SearchClient(server->list_clients, &client_addr);
		if ( search_client == NULL) {
			printf("Это оказался новый пользователь\n");
			struct Client new_client;
			InitClient(&new_client, &client_addr);
			AddNode(server->list_clients, &new_client);
			SendMsg(&new_client, server->fd, server->addr.sin_port, server->ip_addr, msg);
		}else{
			printf("Это оказался старый пользователь (msg_count = %d)\n", search_client->msg_count);
			search_client->msg_count++;
			SendMsg(search_client, server->fd, server->addr.sin_port, server->ip_addr, msg);
		}

		printf("\n");
	}
}
