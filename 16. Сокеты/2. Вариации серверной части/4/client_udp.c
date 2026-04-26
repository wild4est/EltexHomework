#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "ServerStuff.h"

#define SOCKET_PATH_FCTS "/tmp/test_socket_fcts" //from client to server
//#define SIZE_BUFF 10

void main(){
	int fd;
	if ((fd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_un addr_rd;
        addr_rd.sun_family = AF_LOCAL;
       	strncpy(addr_rd.sun_path, SOCKET_PATH_FCTS, sizeof(addr_rd.sun_path));

	if (bind(fd, (struct sockaddr*)&addr_rd, sizeof(addr_rd)) < 0) {
                printf("Не удалось забиндить сокет по адресу %s\n", SOCKET_PATH_FCTS);
                close(fd);
                return;
        }

        printf("Клиент поднял сокет для чтения по адресу %s\n", SOCKET_PATH_FCTS);

	struct sockaddr_un addr_wr;
        addr_wr.sun_family = AF_LOCAL;
       	strncpy(addr_wr.sun_path, SOCKET_PATH_UDP, sizeof(addr_wr.sun_path));

	printf("Клиент подключился по адресу %s\n", SOCKET_PATH_UDP);

	socklen_t len_addr_wr = sizeof(addr_wr);
	socklen_t len_addr_rd = sizeof(addr_rd);
	char* msg = "Hi!";
	sendto(fd, msg, sizeof(msg), 0, (struct sockaddr *)&addr_wr, len_addr_wr);
	printf("Клиент отправил сообщение серверу: %s\n", msg);

	char buff[SIZE_BUFF];
	if (recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr *)&addr_rd, &len_addr_rd) < 0) {
		printf("Не удалось принять сообщение\n");
		close(fd);
		return;
	}
	printf("Клиент принял сообщение от сервера: %s\n", buff);

	close(fd);
	unlink(SOCKET_PATH_FCTS);
}
