#include "ServerStuff.h"
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>

#define TIMEOUT 1000
#define MAX_EVENTS 2
#define MAX_COUNT_CLIENTS 10
#define QUEUE_PATH "/tmp_queue"

int fd_serv_tcp = -1;
int fd_serv_udp = -1;
int fd_epoll = -1;

void SendMsgTCP(int fd_client){
	time_t curtime;
	time(&curtime);
       	char* msg = ctime(&curtime);
        send(fd_client, msg, strlen(msg), 0);
        printf("Клиенту[%d] отправленно собщение %s\n", fd_client, msg);
	close(fd_client);
}

void SendMsgUDP(int fd_serv){
	char buff[SIZE_BUFF];
	struct sockaddr_un client_addr;
	socklen_t len_client_addr = sizeof(client_addr);
	if (recvfrom(fd_serv, buff, sizeof(buff), 0, (struct sockaddr*)&client_addr, &len_client_addr) < 0) {
		printf("Не удалось принять сообщение\n");
		return;
	}
	printf("Сервер принял сообщение от клиента: %s\n", buff);

	time_t curtime;
	time(&curtime);
       	char* msg = ctime(&curtime);
	sendto(fd_serv, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, len_client_addr);
	printf("Сервер отправил сообщение клиенту: %s\n", msg);
}

void StartServerTCP(int* fd_serv){
	struct sockaddr_un addr;
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, SOCKET_PATH_TCP, sizeof(addr.sun_path));
	if ((bind(*fd_serv, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		printf("[X] Не удалось забиндить сокет по адресу %s\n", SOCKET_PATH_TCP);
		close(*fd_serv);
		return;
	}

	if ((listen(*fd_serv, 3)) < 0) {
		printf("[X] Не удалось прослушать\n");
		close(*fd_serv);
		return;
	}

	printf("TCP Сервер[%d] поднят\n", *fd_serv);
}

void StartServerUDP(int* fd_serv){
        struct sockaddr_un addr_wr;
        addr_wr.sun_family = AF_LOCAL;
        strncpy(addr_wr.sun_path, SOCKET_PATH_UDP, sizeof(addr_wr.sun_path));

        if (bind(*fd_serv, (struct sockaddr*)&addr_wr, sizeof(addr_wr)) < 0) {
                printf("Не удалось забиндить сокет по адресу %s\n", SOCKET_PATH_UDP);
                close(*fd_serv);
                return;
        }

        printf("UDP Сервер[%d] поднят\n", *fd_serv);
}

void AddEventToEpoll(struct epoll_event* event, int fd_epoll, int fd_serv){
	(*event).events = EPOLLIN;
	(*event).data.fd = fd_serv;
	epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_serv, event);
}

static void CloseServerHandler(){
        printf("[!] Получен сигнал SIGINT. Сервер завершает свою работу.\n");
        if (fd_serv_tcp >= 0) {
                close(fd_serv_tcp);
                unlink(SOCKET_PATH_TCP);
                fd_serv_tcp = -1;
        }
	
	if (fd_serv_udp >= 0) {
		close(fd_serv_udp);
		unlink(SOCKET_PATH_UDP);
		fd_serv_udp = -1;
        }

	if (fd_epoll != -1){
		close(fd_epoll);
	}
        exit(EXIT_SUCCESS);
}

void InitSignalProcessing(){
        struct sigaction act;
        act.sa_handler = CloseServerHandler;
        sigaction(SIGINT, &act, NULL);
}

void main(){
	if ((fd_serv_tcp = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
                printf("Не удалось создать файловый дескриптор\n");
                return;
        }
	if ((fd_serv_udp = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
                printf("Не удалось создать файловый дескриптор\n");
                return;
        }

	InitSignalProcessing();	

	fd_epoll = epoll_create1(0);
	struct epoll_event event_tcp, event_udp;
	AddEventToEpoll(&event_tcp, fd_epoll, fd_serv_tcp);
	AddEventToEpoll(&event_udp, fd_epoll, fd_serv_udp);

	StartServerTCP(&fd_serv_tcp);
	StartServerUDP(&fd_serv_udp);
	
	while(1){
		struct epoll_event events[MAX_EVENTS];
		int num_ready = epoll_wait(fd_epoll, events, MAX_EVENTS, TIMEOUT);
		
		if (num_ready == 0) {
			continue;
		}

		for (int i=0; i<MAX_EVENTS; i++) {
			if (events[i].events & EPOLLIN) {
				printf("[!] К серверу[%d] кто-то пытается подключиться\n", events[i].data.fd);

				if (events[i].data.fd == fd_serv_tcp) {
					printf("Это оказался TCP клиент!\n");
					int new_fd;
					if ((new_fd = accept(events[i].data.fd, NULL, NULL)) < 0) {
						printf("Не удалось создать новый файловый дескриптор\n");
						close(events[i].data.fd);
						return;
					}	
					SendMsgTCP(new_fd);
				}else if (events[i].data.fd = fd_serv_udp) {
					printf("Это оказался UDP клиент!\n");
					SendMsgUDP(events[i].data.fd);
				}

			}
		}
	}
}
