#include "ServerStuff.h"
#include <pthread.h>
#include <signal.h>

#define MAX_COUNT_CLIENTS 10

int fd_serv = -1;

void* MessagingListener(void* args){
	int* fd_client = (int*)args;

	while (1) {
		if (*fd_client < 0) {
			continue;
		}

		time_t curtime;
	        time(&curtime);
        	char* msg = ctime(&curtime);
        	send(*fd_client, msg, strlen(msg), 0);
        	printf("Клиенту[%d] отправленно собщение %s\n", *fd_client, msg);

		close(*fd_client);
		
		*fd_client = -1;
	}
}

int FindFreeThread(int* fd_clients, int n){
	for (int i=0; i<n; i++) {
		if (fd_clients[i] < 0) {
			return i;
		}
	}
	return -1;
}

static void CloseServerHandler(){
        printf("[!] Получен сигнал SIGINT. Сервер завершает свою работу.\n");
        if (fd_serv >= 0) {
                close(fd_serv);
                unlink(SOCKET_PATH);
                fd_serv = -1;
        }
        exit(EXIT_SUCCESS);
}

void InitSignalProcessing(){
        struct sigaction act;
        act.sa_handler = CloseServerHandler;
        sigaction(SIGINT, &act, NULL);
}

void main(){
	fd_serv = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (fd_serv < 0) {
		printf("[X] Не удалось создать файловый дескриптор\n");
		return;
	}
	
	InitSignalProcessing();

	struct sockaddr_un addr;
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
	if ((bind(fd_serv, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		printf("[X] Не удалось забиндить сокет по адресу %s\n", SOCKET_PATH);
		close(fd_serv);
		return;
	}

	if ((listen(fd_serv, 3)) < 0) {
		printf("[X] Не удалось прослушать\n");
		close(fd_serv);
		return;
	}

	printf("Сервер поднят\n");

	int fd_clients[MAX_COUNT_CLIENTS];
	pthread_t threads[MAX_COUNT_CLIENTS];

	for (int i=0; i<MAX_COUNT_CLIENTS; i++) {
		fd_clients[i] = -1;
		pthread_create(&threads[i], NULL, MessagingListener, &fd_clients[i]);
	}

	while(1){
		int new_fd = accept(fd_serv, NULL, NULL);
		if (new_fd < 0) {
			printf("[X] Не удалось принять нового пользователя\n");
			continue;
		}

		int free_thread_i = FindFreeThread(fd_clients, MAX_COUNT_CLIENTS);
		if (free_thread_i < 0) {
			printf("[!] В данный момент времени все потоки заняты\n");
			int answer = 0;
			send(new_fd, &answer, sizeof(answer), 0);
			continue;
		}
		
		int answer = 1;
		send(new_fd, &answer, sizeof(answer), 0);
		fd_clients[free_thread_i] = new_fd;
	}
	

}
