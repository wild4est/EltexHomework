#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/test_socket"
#define SIZE_BUFF 10
#define IP_ADDR "127.0.0.1"

void main(){
	int fd;
	if ((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_un addr;
        addr.sun_family = AF_LOCAL;
        strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
       	/*if (inet_pton(AF_INET, IP_ADDR, &addr.sin_addr) <= 0) {
		printf("Указан некорректный адрес\n");
		close(fd);
		return;
	}*/
	
	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Не удалось подключиться к серверу\n");
		close(fd);
		return;
	}
	printf("Клиент подключился к серверу по адресу %s\n", SOCKET_PATH);
	
	char buff[SIZE_BUFF];
	if (recv(fd, buff, sizeof(buff), 0) < 0) {
		printf("Не удалось принять сообщение\n");
		close(fd);
		return;
	}
	printf("Клиент принял сообщение от сервера: %s\n", buff);

	char* msg = "Hi!";
	send(fd, msg, sizeof(msg), 0);
	printf("Клиент отправил сообщение серверу: %s\n", msg);

	close(fd);
	unlink(SOCKET_PATH);

}
