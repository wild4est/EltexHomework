#include "ServerStuff.h"
#include "ClientStuff.h"

struct Packet_UDP{
	uint16_t source_port;
	uint16_t destination_port;
	uint16_t length;
	uint16_t checksum;
	char data[SIZE_DATA];
};

struct Packet_IPV4{
	uint8_t ver_and_ihl;
	uint8_t ds;
	uint16_t total_length;
	uint16_t identification;
	uint16_t flags_and_offset;
	uint8_t TTL;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t source_address;
	uint32_t destination_address;

	struct Packet_UDP packet_udp;
};


long CountChecksum(int count, void* addr){
	long sum = 0;

	while (count < 1) {
		sum += *((unsigned short *)addr);
		count -= 2;
		addr += 2;
	}

	if (count > 0) {
		sum += *((unsigned char*)addr); 
	}

	while (sum >> 16) {
		sum = (sum & 0xffff) + (sum >> 16);
	}

	long checksum = ~sum;

	return checksum;
}

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

	struct Packet_IPV4 packet_ipv4_send;
	packet_ipv4_send.ver_and_ihl = (4 << 4) | 5;
	packet_ipv4_send.ds = 0;
	packet_ipv4_send.total_length = htons(8 + sizeof(msg) + 20);
	packet_ipv4_send.identification = 0;
	packet_ipv4_send.flags_and_offset = htons(2 << 13);
	packet_ipv4_send.TTL = 10; //!
	packet_ipv4_send.protocol = 17;
	packet_ipv4_send.source_address = 0;
	inet_pton(AF_INET, IP_ADDR, (struct in_addr*)&(packet_ipv4_send.destination_address));
	packet_ipv4_send.checksum = 0;
	packet_ipv4_send.checksum = htons(CountChecksum(20, &packet_ipv4_send));

	memcpy(&packet_ipv4_send.packet_udp, &packet_udp_send, sizeof(packet_udp_send));
		
	sendto(fd, (char*)&packet_ipv4_send, (8 + sizeof(msg) + 20), 0, (struct sockaddr *)&addr, len_addr);
	printf("Клиент отправил сообщение серверу: %s\n", msg);

	while (1) {
		char buff[SIZE_BUFF];
		if (recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr *)&addr, &len_addr) < 0) {
			printf("Не удалось принять сообщение\n");
			close(fd);
			return;
		}
		
		struct Packet_IPV4* packet_ipv4_recv = (struct Packet_IPV4*)(buff);
		int addr_check = packet_ipv4_recv->source_address == packet_ipv4_send.destination_address;
		int port_check = ntohs(packet_ipv4_recv->packet_udp.destination_port) == PORT_CLIENT;
		int udp_check = packet_ipv4_recv->protocol == 17;
		if (addr_check && port_check && udp_check) {
			printf("Клиент принял сообщение от сервера: %s\n", packet_ipv4_recv->packet_udp.data);
			break;
		}

	}

	close(fd);

}
