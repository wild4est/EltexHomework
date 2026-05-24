#include <linux/if_packet.h>
#include <net/ethernet.h>
#include "ServerStuff.h"
#include "ClientStuff.h"

#define SIZE_BUFF 64
#define SIZE_DATA 10
#define PORT_CLIENT 45289

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
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t source_address;
	uint32_t destination_address;

	struct Packet_UDP packet_udp;
};

struct Packet_Ethernet{
	uint16_t padding;
	uint8_t mac_destination[6];
	uint8_t mac_source[6];
	uint16_t ethertype;
	struct Packet_IPV4 packet_ipv4;
};

long CountChecksum(int count, void* addr){
	long sum = 0;

	while (count > 1) {
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

void FillPacketUDP(struct Packet_UDP* packet_udp, char* msg){
	packet_udp->source_port = htons(PORT_CLIENT);
	packet_udp->destination_port = htons(PORT);
	packet_udp->length = htons(8 + strlen(msg));
	packet_udp->checksum = 0;
	strcpy(packet_udp->data, msg);
}

void FillPacketIPV4(struct Packet_IPV4* packet_ipv4, struct Packet_UDP packet_udp){
	packet_ipv4->ver_and_ihl = (4 << 4) | 5;
	packet_ipv4->ds = 0;
	packet_ipv4->total_length = htons(ntohs(packet_udp.length) + 20);
	packet_ipv4->identification = 0;
	packet_ipv4->flags_and_offset = htons(2 << 13);
	packet_ipv4->ttl = 255; //!
	packet_ipv4->protocol = 17;
	inet_pton(AF_INET, IP_ADDR_SRC, (struct in_addr*)&(packet_ipv4->source_address));
	inet_pton(AF_INET, IP_ADDR_DES, (struct in_addr*)&(packet_ipv4->destination_address));
	packet_ipv4->checksum = 0;
	packet_ipv4->checksum = CountChecksum(20, packet_ipv4);

	memcpy(&packet_ipv4->packet_udp, &packet_udp, sizeof(packet_udp));	
}

int FromStringToMac(const char* mac, uint8_t* values){
    if (6 == sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &values[0], &values[1], &values[2], &values[3],
			    &values[4], &values[5])){
        return 1;
    }else{
        return 0;
    }
}

void FillPacketEthernet(struct Packet_Ethernet* packet_ethernet, struct Packet_IPV4 packet_ipv4){
	memset(packet_ethernet, 0, sizeof(*packet_ethernet));
	FromStringToMac(MAC_DESTIN, packet_ethernet->mac_destination);
	FromStringToMac(MAC_SOURCE, packet_ethernet->mac_source);
	packet_ethernet->ethertype = htons(ETH_P_IP);
	memcpy(&packet_ethernet->packet_ipv4, &packet_ipv4, sizeof(packet_ipv4));
}

void main(){
	int fd;
	if ((fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_ll addr;
        addr.sll_family = AF_PACKET;
       	addr.sll_protocol = htons(ETH_P_ALL);
	addr.sll_ifindex = ETH_INDEX;
	addr.sll_halen = ETH_ALEN;
	uint8_t mac_addr_dest[6];
	FromStringToMac(MAC_DESTIN, mac_addr_dest);
	memcpy(addr.sll_addr, mac_addr_dest, sizeof(mac_addr_dest));
	socklen_t len_addr = sizeof(addr); 

	char* msg = "Hi!";
	struct Packet_UDP packet_udp_send;
	FillPacketUDP(&packet_udp_send, msg);
	
	struct Packet_IPV4 packet_ipv4_send;
	FillPacketIPV4(&packet_ipv4_send, packet_udp_send);

	struct Packet_Ethernet packet_ethernet_send;
	FillPacketEthernet(&packet_ethernet_send, packet_ipv4_send);

	ssize_t recv = sendto(fd,((char*)&packet_ethernet_send) + 2, (8 + strlen(msg) + 20 + 14), 0, (struct sockaddr *)&addr, len_addr);
	
	if (recv == -1) {
		printf("Не удалось отправить сообщение\n");
	}
	printf("Клиент отправил сообщение серверу: %s\n", msg);

	while (1) {
		char buff[SIZE_BUFF + 2];
		if (recvfrom(fd, buff + 2, sizeof(buff), 0, (struct sockaddr *)&addr, &len_addr) < 0) {
			printf("Не удалось принять сообщение\n");
			close(fd);
			return;
		}
		
		struct Packet_Ethernet* packet_ethernet_recv = (struct Packet_Ethernet*)(buff);
		struct Packet_IPV4 packet_ipv4_recv;
		memcpy(&packet_ipv4_recv, &(packet_ethernet_recv->packet_ipv4), sizeof(packet_ethernet_recv->packet_ipv4));
		int addr_check = packet_ipv4_recv.source_address == packet_ipv4_send.destination_address;
		int port_check = ntohs(packet_ipv4_recv.packet_udp.destination_port) == PORT_CLIENT;
		int udp_check = packet_ipv4_recv.protocol == 17;
		
		if (addr_check && port_check && udp_check) {
			printf("Клиент принял сообщение от сервера: %s\n", packet_ipv4_recv.packet_udp.data);
			break;
		}

	}

	close(fd);

}
