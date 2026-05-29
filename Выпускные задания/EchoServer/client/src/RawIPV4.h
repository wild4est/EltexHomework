#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE_DATA 255

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

void FillPacketUDP(struct Packet_UDP* packet_udp, int port_server, int port_client, char* msg);
//void FillPacketIPV4(struct Packet_IPV4* packet_ipv4, const char* ip_addr_server, struct Packet_UDP packet_udp);
void FillPacketIPV4(struct Packet_IPV4* packet_ipv4, uint32_t ip_addr_server, uint32_t ip_addr_source, struct Packet_UDP* packet_udp);
long CountChecksum(int count, void* addr);
int PacketIPV4Check(struct Packet_IPV4* packet_ipv4, uint32_t source_address, int source_port);
