#include "ServerStuff.h"

#define SIZE_BUFF 64
#define SIZE_DATA 10
#define PORT_CLIENT 45289
#define IP_ADDR "127.0.0.1"



struct Packet_UDP{
	uint16_t source_port;
	uint16_t destination_port;
	uint16_t length;
	uint16_t checksum;
	char data[SIZE_DATA];
};

/*
struct Packet_IPV4{
	uint8_t version;
	uint8_t ihl;
	uint16_t dhcp;
	uint32_t total_length;
	uint32_t identification;
	uint	

	struct Packet_UDP;
};*/


void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	int opt = 1;
	setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));
		
	struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
       	if (inet_pton(AF_INET, IP_ADDR, &addr.sin_addr) <= 0) {
		printf("Указан некорректный адрес\n");
		close(fd);
		return;
	}
	socklen_t len_addr = sizeof(addr);

	char* msg = "Hi!";

	struct Packet_UDP packet_udp_send;
	packet_udp_send.source_port = htons(PORT_CLIENT);
	packet_udp_send.destination_port = htons(PORT);
	packet_udp_send.length = htons(8 + sizeof(msg));
	packet_udp_send.checksum = 0;
	strcpy(packet_udp_send.data, msg);
	
	sendto(fd, (char*)&packet_udp_send, sizeof(packet_udp_send), 0, (struct sockaddr *)&addr, len_addr);
	printf("Клиент отправил сообщение серверу: %s\n", msg);

	while (1) {
		char buff[SIZE_BUFF];
		if (recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr *)&addr, &len_addr) < 0) {
			printf("Не удалось принять сообщение\n");
			close(fd);
			return;
		}
		
		struct Packet_UDP* packet_udp_recv = (struct Packet_UDP*)(buff + 20);
		if (ntohs(packet_udp_recv->destination_port) == PORT_CLIENT) {
			printf("Клиент принял сообщение от сервера: %s\n", packet_udp_recv->data);
			break;
		}

	}

	close(fd);

}
