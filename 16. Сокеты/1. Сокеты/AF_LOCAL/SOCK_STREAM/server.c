#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/test_socket"
#define SIZE_BUFF 10

void main(){
	int fd;
	if ((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_un addr;
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);
	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Не удалось забиндить сокет по адресу %s\n", SOCKET_PATH);
		close(fd);
		return;
	}

	if (listen(fd, 3) < 0) {
		printf("Не удалось прослушать\n");
		close(fd);
		return;
	}
	printf("Сервер поднят\n");

	int new_fd;
	//socklen_t len_addr = sizeof(addr);
	//if (new_fd = accept(fd, (struct sockaddr*)&addr, &len_addr) < 0) {
	if ((new_fd = accept(fd, NULL, NULL)) < 0) {
		printf("Не удалось создать новый файловый дескриптор\n");
		close(fd);
		return;
	}
	
	char* msg = "Hello!";
	send(new_fd, msg, strlen(msg), 0);
	printf("Сервер отправил сообщение клиенту: %s\n", msg);

	char buff[SIZE_BUFF];
	if (recv(new_fd, buff, sizeof(buff), 0) < 0) {
		printf("Не удалось принять сообщение\n");
		return;
	}
	printf("Сервер принял сообщение от клиента: %s\n", buff);

	close(new_fd);
	close(fd);
	unlink(SOCKET_PATH);	
}
