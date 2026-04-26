#include "ServerStuff.h"
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COUNT_CLIENTS 10
#define QUEUE_PATH "/tmp_queue"

int fd_serv = -1;

void* MessagingListener(void* args){
	mqd_t* mqd = (mqd_t*)args;

	while (1) {
		int fd_client;
		mq_receive(*mqd, (char*)(&fd_client), sizeof(fd_client) ,NULL);

		time_t curtime;
	        time(&curtime);
       		char* msg = ctime(&curtime);
        	send(fd_client, msg, strlen(msg), 0);
        	printf("Клиенту[%d] отправленно собщение %s\n", fd_client, msg);

		close(fd_client);
	}
}

int FindFreeThread(mqd_t mqd){
	struct mq_attr attr;
	if (mq_getattr(mqd, &attr) == -1){
		return -2;
	}
	if (attr.mq_curmsgs < MAX_COUNT_CLIENTS) {
		return 1;
	}
	return -1;
}

void InitQueue(mqd_t* mqd){
	struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = MAX_COUNT_CLIENTS;
        attr.mq_msgsize = sizeof(int);
        attr.mq_curmsgs = 0;

	*mqd = mq_open(QUEUE_PATH, O_RDWR | O_CREAT, 0666, &attr);
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

	mqd_t mqd;
	InitQueue(&mqd);

	pthread_t threads[MAX_COUNT_CLIENTS];

	for (int i=0; i<MAX_COUNT_CLIENTS; i++) {
		pthread_create(&threads[i], NULL, MessagingListener, &mqd);
	}

	while(1){
		int new_fd = accept(fd_serv, NULL, NULL);
		if (new_fd < 0) {
			printf("[X] Не удалось принять нового пользователя\n");
			continue;
		}

		if (FindFreeThread(mqd) < 0) {
			printf("[!] Не удалось обслужить нового клиента: возможно сервер переполнен\n");
			char* answer = ERROR_CODE;
			send(new_fd, &answer, sizeof(answer), 0);
			continue;
		}
		
		mq_send(mqd, (char*)&new_fd, sizeof(new_fd), 2) == -1;
					
	}
}
