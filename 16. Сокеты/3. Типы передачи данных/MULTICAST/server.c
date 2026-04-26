#include <arpa/inet.h>
#include "ServerStuff.h"

#define MULTICAST_ADDR "224.0.0.1"
#define MSG_COUNT 10

void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct ip_mreqn mreqn;
	mreqn.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
	mreqn.imr_address.s_addr = INADDR_ANY;
	setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(mreqn));
	
	printf("Сервер поднят по порту %d\n", PORT);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	socklen_t len_addr = sizeof(addr);
	
	for (int i=0; i < MSG_COUNT; i++) {
		char* msg = "Hello!";
		sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&addr, len_addr);
		printf("Сервер отправил сообщение: %s\n", msg);
	}
	close(fd);
	
}
