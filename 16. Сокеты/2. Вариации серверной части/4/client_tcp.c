#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "ServerStuff.h"

void main(){
	int fd;
	if ((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_un addr;
        addr.sun_family = AF_LOCAL;
        strncpy(addr.sun_path, SOCKET_PATH_TCP, sizeof(addr.sun_path));
 
	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Не удалось подключиться к серверу\n");
		close(fd);
		return;
	}
	printf("Клиент подключился к серверу по адресу %s\n", SOCKET_PATH_TCP);
	
	char buff[SIZE_BUFF];
	if (recv(fd, buff, sizeof(buff), 0) < 0) {
		printf("Не удалось принять сообщение\n");
		close(fd);
		return;
	}
	printf("Клиент принял сообщение от сервера: %s\n", buff);

	close(fd);
}
