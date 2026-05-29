#include "RawIPV4.h"

void FillPacketUDP(struct Packet_UDP* packet_udp, int port_server, int port_client, char* msg){
	packet_udp->source_port = htons(port_client);
	packet_udp->destination_port = htons(port_server);
	packet_udp->length = htons(8 + strlen(msg));
	packet_udp->checksum = 0;
	strcpy(packet_udp->data, msg);
}

void FillPacketIPV4(struct Packet_IPV4* packet_ipv4, uint32_t ip_addr_server, uint32_t ip_addr_source, struct Packet_UDP* packet_udp){
	packet_ipv4->ver_and_ihl = (4 << 4) | 5;
	packet_ipv4->ds = 0;
	packet_ipv4->total_length = htons(ntohs(packet_udp->length) + 20);
	packet_ipv4->identification = 0;
	packet_ipv4->flags_and_offset = htons(2 << 13);
	packet_ipv4->ttl = 255; //!
	packet_ipv4->protocol = 17;
	packet_ipv4->source_address = ip_addr_source;
	packet_ipv4->destination_address = ip_addr_server;
	//packet_ipv4->source_address = 0;
	//inet_pton(AF_INET, IP_ADDR_SRC, (struct in_addr*)&(packet_ipv4->source_address));
	//inet_pton(AF_INET, ip_addr_server, (struct in_addr*)&(packet_ipv4->destination_address));
	packet_ipv4->checksum = 0;
	packet_ipv4->checksum = CountChecksum(20, packet_ipv4);

	memcpy(&packet_ipv4->packet_udp, packet_udp, sizeof(*packet_udp));
}

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


int PacketIPV4Check(struct Packet_IPV4* packet_ipv4, uint32_t source_address, int source_port){
	int addr_check = source_address == packet_ipv4->destination_address;
	int port_check = ntohs(packet_ipv4->packet_udp.destination_port) == source_port;
	
	if (addr_check && port_check) {
		return 1;
	}else{
		return 0;
	}
}
