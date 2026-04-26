#include "ServerStuff.h"
#define MSG_COUNT 10

void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
	
	printf("Сервер поднят по порту %d\n", PORT);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_BROADCAST;
	addr.sin_port = htons(PORT);
	socklen_t len_addr = sizeof(addr);

	for (int i=0; i<MSG_COUNT; i++) {
		char* msg = "Hello!";
		sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&addr, len_addr);
		printf("Сервер отправил сообщение: %s\n", msg);
	}	
	close(fd);
	
}
