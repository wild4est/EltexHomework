#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE_BUFF 10
#define PORT 49134
#define IP_ADDR "127.0.0.1"

void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
       	if (inet_pton(AF_INET, IP_ADDR, &addr.sin_addr) <= 0) {
		printf("Указан некорректный адрес\n");
		close(fd);
		return;
	}
	
	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Не удалось подключиться к серверу\n");
		close(fd);
		return;
	}
	printf("Клиент подключился к серверу по порту %d\n", PORT);
	
	socklen_t len_addr = sizeof(addr);
	char* msg = "Hi!";
	sendto(fd, msg, sizeof(msg), 0, (struct sockaddr *)&addr, len_addr);
	printf("Клиент отправил сообщение серверу: %s\n", msg);

	char buff[SIZE_BUFF];
	if (recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr *)&addr, &len_addr) < 0) {
		printf("Не удалось принять сообщение\n");
		close(fd);
		return;
	}
	printf("Клиент принял сообщение от сервера: %s\n", buff);


	close(fd);

}
