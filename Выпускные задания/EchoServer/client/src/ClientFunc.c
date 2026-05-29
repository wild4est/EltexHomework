#include "ClientFunc.h"

int InitClient(struct Client* client, int port_client, int port_server, const char* ip_addr, const char* ip_addr_server){
        if ((client->fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
		int err = errno;
		printf("[X] Не удалось создать файловый дескриптор %d\n", err);
		return -1;
        }

        client->addr.sin_family = AF_INET;
        client->addr.sin_port = htons(port_server);
        if (inet_pton(AF_INET, IP_ADDR, &(client->addr.sin_addr)) <= 0) {
                printf("[X] Не удалось забиндить порт %d\n", port_server);
		close(client->fd);
                return -1;
        }
        
        if (connect(client->fd, (struct sockaddr*)&(client->addr), sizeof(client->addr)) < 0) {
                printf("[X] Не удалось законектиться\n");
		close(client->fd);
                return -1;
        }
	
	int recv;
	if ((recv = inet_pton(AF_INET, ip_addr_server, (struct in_addr*)&client->ip_addr_server)) <= 0) {
		printf("[X] Не удалось записать адресс сервера %d\n", recv);
		close(client->fd);
		return -1;
	}
	
	if (inet_pton(AF_INET, ip_addr, (struct in_addr*)&client->ip_addr) <= 0) {
		printf("[X] Не удалось записать адресс клиента\n");
		close(client->fd);
		return -1;
	}
	
	client->port = port_client;
	client->port_server = port_server;

	client->list_msg = InitList(10);

	return 0;
}

int SendMsg(struct Client* client, char* msg){
	socklen_t len_addr = sizeof(client->addr);
	char buff[SIZE_BUFF - 10];
	strcpy(buff, msg);

	struct Packet_UDP packet_udp;
	FillPacketUDP(&packet_udp, client->port_server, client->port, buff);

	struct Packet_IPV4 packet_ipv4;
	FillPacketIPV4(&packet_ipv4, client->ip_addr_server, client->ip_addr, &packet_udp);
	
	ssize_t recv_send = sendto(client->fd, (char*)&packet_ipv4, (sizeof(buff) + 28) , 0, (struct sockaddr *)&(client->addr), len_addr);
	if (recv_send == -1) {
		//printf("Не удалось отправить сообщение");
		return -1;
	}

	//printf("Клиент отправил сообщение серверу: %s\n", msg);

	return 0;
}

int GetMsg(struct Client* client){
	socklen_t len_addr = sizeof(client->addr);
	
	while (1) {
		char buff[SIZE_BUFF];
		ssize_t recv_get = recvfrom(client->fd, buff, sizeof(buff), 0, (struct sockaddr *)&(client->addr), &len_addr);
		if ( recv_get == -1 ) {
			return -1;
		}

		struct Packet_IPV4* packet_ipv4 = (struct Packet_IPV4*)(buff);
		if ( PacketIPV4Check(packet_ipv4, client->ip_addr, client->port) ) {
			if ( client->list_msg->length >= client->list_msg->max_length ) {
				DeleteFirstMsg(client->list_msg);
			}
			AddNode(client->list_msg, buff);	
			break;
		}

		
	}
	
	return 0;
}

int IsItExitMsg(char* msg){
	if (strcmp(msg, EXIT_CODE) == 0) {
		return 1;
	}else{
		return 0;
	}

}
