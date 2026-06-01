#include "ClientFunc.h"

int InitClient(struct Client* client, int port_client, int port_server, const char* ip_addr_client, const char* ip_addr_server){
        if ((client->fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
                int err = errno;
		printf("Не удалось создать файловый дескриптор %d\n", err);
                return -1;
        }
	int opt = 1;
	setsockopt(client->fd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));

        client->addr.sin_family = AF_INET;
        client->addr.sin_port = htons(port_server);
        if (inet_pton(AF_INET, ip_addr_server, &(client->addr.sin_addr)) <= 0) {
                printf("Указан некорректный адрес сервера\n");
                close(client->fd);
                return -1;
        }
        
	client->port = htons(port_client);
	if (inet_pton(AF_INET, ip_addr_client, &(client->ip_addr)) <= 0) {
                printf("Указан некорректный адрес клиента\n");
                close(client->fd);
                return -1;
        }
	client->list_msg = InitList(20);

	return 0;
}

void FreeClient(struct Client* client) {
	close(client->fd);
	FreeList(client->list_msg);
}


int SendMsg(struct Client* client, char* msg){
	socklen_t len_addr = sizeof(client->addr);
	struct Packet_UDP packet_udp;
	FillPacketUDP(&packet_udp, client->port, client->addr.sin_port, msg);
	struct Packet_IPV4 packet_ipv4;
	FillPacketIPV4(&packet_ipv4, client->ip_addr, client->addr.sin_addr.s_addr, &packet_udp);
	
	ssize_t recv_send = sendto(client->fd, (char*)&packet_ipv4, sizeof(packet_ipv4), 0, (struct sockaddr *)&(client->addr), len_addr);
	if (recv_send == -1) {
		//printf("Не удалось отправить сообщение");
		return -1;
	}

	//printf("Клиент отправил сообщение серверу: %s\n", msg);

	return 0;
}

int GetMsg(struct Client* client){
	while (1) {
		char buff[SIZE_BUFF];
		ssize_t recv_get = recvfrom(client->fd, buff, sizeof(buff), 0, NULL, NULL);
		if ( recv_get == -1 ) {
			return -1;
		}

		struct Packet_IPV4* packet_ipv4 = (struct Packet_IPV4*)(buff);
		
		if ( PacketIPV4Check(packet_ipv4, client->ip_addr, client->port) ) {
			if ( IsItExitMsg(packet_ipv4->packet_udp.data) ) {
				return 1;
			}
			if ( client->list_msg->length >= client->list_msg->max_length ) {
				DeleteFirstMsg(client->list_msg);
			}

			AddNode(client->list_msg, packet_ipv4->packet_udp.data);
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
