#include "ServerStuff.h"

#define SIZE_BUFF 10

void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Не удалось забиндить порт %d\n", PORT);
		close(fd);
		return;
	}

	printf("Сервер поднят\n");

	char buff[SIZE_BUFF];
	struct sockaddr_in client_addr;
	socklen_t len_client_addr = sizeof(client_addr);
	if (recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr*)&client_addr, &len_client_addr) < 0) {
		printf("Не удалось принять сообщение\n");
		return;
	}
	printf("Сервер принял сообщение от клиента: %s\n", buff);

	char* msg = "Hello!";
	sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, len_client_addr);
	printf("Сервер отправил сообщение клиенту: %s\n", msg);

	
	close(fd);
	
}
