#include <signal.h>

#include "src/ServerFunc.h"

struct Server* serv = NULL;

static void CloseServerHandler() {
	if (serv == NULL) {
		printf(
		    "[X] CloseClientHandler: структура клиента равна NULL\n");
		return;
	}

	mq_close(serv->mqd_new_clients);
	mq_close(serv->mqd_messaging);
	//	mq_close(serv->mqd_broadcast);
	mq_close(serv->mqd_exit);

	FreeListMsg(serv->list_msgs);
	FreeListClient(serv->list_clients);

	printf("Сервер окончил свою работу\n");
	exit(EXIT_SUCCESS);
}

void InitSignalProcessing() {
	struct sigaction act;
	act.sa_handler = CloseServerHandler;
	sigaction(SIGINT, &act, NULL);
}

void main() {
	serv = InitServer();
	InitSignalProcessing();

	pthread_t thread_for_new_client;
	pthread_t thread_messaging;
	pthread_t thread_exit;

	pthread_create(&thread_for_new_client, NULL, NewClientListener, serv);
	pthread_create(&thread_messaging, NULL, MessagingListener, serv);
	pthread_create(&thread_exit, NULL, ExitListener, serv);

	pthread_join(thread_for_new_client, NULL);
	pthread_join(thread_messaging, NULL);
	pthread_join(thread_exit, NULL);
}

