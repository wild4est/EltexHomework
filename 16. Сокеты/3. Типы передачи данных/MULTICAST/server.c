#include "ServerStuff.h"

#define MSG_COUNT 10

void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}
	
	struct in_addr opt_addr;
	opt_addr.s_addr = INADDR_ANY;

	setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char*)&opt_addr, sizeof(opt_addr));
	
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
