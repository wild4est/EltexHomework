#include "ServerStuff.h"
#include <pthread.h>
#include <signal.h>

#define MAX_COUNT_CLIENT 10

int fd_serv = -1;

void* MessagingListener(void* args){
	int fd_client = *( (int*)args );
	
	time_t curtime;
	time(&curtime);
	char* msg = ctime(&curtime);
	send(fd_client, msg, strlen(msg), 0);
	printf("Клиенту[%d] отправленно собщение %s\n", fd_client, msg);

	close(fd_client);
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

	while(1){
		int new_fd = accept(fd_serv, NULL, NULL);
		if (new_fd < 0) {
			printf("[X] Не удалось принять нового пользователя\n");
		}else{
			printf("Принят новый пользователь[%d]\n", new_fd);
			pthread_t thread;
			pthread_create(&thread, NULL, MessagingListener, &new_fd);
		}

	}
}
