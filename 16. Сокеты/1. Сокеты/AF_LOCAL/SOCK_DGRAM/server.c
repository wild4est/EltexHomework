#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

#define SOCKET_PATH_FSTC "/tmp/test_socket_fstc" //from server to client
#define SIZE_BUFF 10

void main(){
	int fd;
	if ((fd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_un addr_wr;
	addr_wr.sun_family = AF_LOCAL;
	strncpy(addr_wr.sun_path, SOCKET_PATH_FSTC, sizeof(addr_wr.sun_path));

	if (bind(fd, (struct sockaddr*)&addr_wr, sizeof(addr_wr)) < 0) {
		printf("Не удалось забиндить сокет по адресу %s\n", SOCKET_PATH_FSTC);
		close(fd);
		return;
	}

	printf("Сервер поднял сокет для чтения по адресу %s\n", SOCKET_PATH_FSTC);
	
	char buff[SIZE_BUFF];
	struct sockaddr_un client_addr;
	socklen_t len_client_addr = sizeof(client_addr);
	if (recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr*)&client_addr, &len_client_addr) < 0) {
		printf("Не удалось принять сообщение\n");
		return;
	}
	printf("Сервер принял сообщение от клиента: %s\n", buff);

	char* msg = "Hello!";
	sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, len_client_addr);
	printf("Сервер отправил сообщение клиенту: %s\n", msg);

	
	//close(new_fd);
	close(fd);
	
}
