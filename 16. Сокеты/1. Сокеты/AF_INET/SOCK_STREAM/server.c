#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 49134
#define SIZE_BUFF 10

void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
	
}
