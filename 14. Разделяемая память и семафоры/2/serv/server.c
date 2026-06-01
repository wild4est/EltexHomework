#include <signal.h>

#include "src/ServerFunc.h"

struct Server* serv = NULL;

static void CloseServerHandler() {
	if (serv == NULL) {
		printf(
		    "[X] CloseClientHandler: структура клиента равна NULL\n");
		return;
	}

	CloseSharedMemory(&(serv->shared_mem_new_client));
	CloseSharedMemory(&(serv->shared_mem_messaging));
	CloseSharedMemory(&(serv->shared_mem_exit));

	UnlinkSharedMemory(SHM_NEW_CLIENT, SEM_NEW_CLIENT_RD,
			   SEM_NEW_CLIENT_WR);
	UnlinkSharedMemory(SHM_MESSAGING, SEM_MESSAGING_RD, SEM_MESSAGING_WR);
	UnlinkSharedMemory(SHM_EXIT, SEM_EXIT_RD, SEM_EXIT_WR);

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

