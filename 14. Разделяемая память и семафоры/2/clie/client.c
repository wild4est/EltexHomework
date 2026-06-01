#include <locale.h>
#include <signal.h>

#include "src/ClientFunc.h"
#include "src/UIFunc.h"

extern struct Workspace workspace;
struct Client* client = NULL;

static void CloseClientHandler() {
	if (client == NULL) {
		return;
	}

	sem_wait(client->shared_mem_exit.sem_write);
	sprintf(client->shared_mem_exit.ptr, "%s", client->name);
	sem_post(client->shared_mem_exit.sem_read);

	char name_shm[SIZE_BUFF_NAME + 1];
	sprintf(name_shm, "/%s", client->name);
	char name_sem_rd[SIZE_BUFF_NAME + 4];
	sprintf(name_sem_rd, "/rd_%s", client->name);
	char name_sem_wr[SIZE_BUFF_NAME + 4];
	sprintf(name_sem_wr, "/wr_%s", client->name);

	CloseSharedMemory(&(client->shared_mem_messaging));
	CloseSharedMemory(&(client->shared_mem_personal));
	CloseSharedMemory(&(client->shared_mem_exit));

	UnlinkSharedMemory(name_shm, name_sem_rd, name_sem_wr);

	FreeList(client->list_clients);
	FreeList(client->list_msgs);
	endwin();
	exit(EXIT_SUCCESS);
}

void InitSignalProcessing() {
	struct sigaction act;
	act.sa_handler = CloseClientHandler;
	sigaction(SIGINT, &act, NULL);
}

void main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	if (argc < 2) {
		printf("[X] Ошибка: нету имени клиента\n");
		return;
	}

	char* name = argv[1];
	client = InitClient(name);
	InitSignalProcessing();

	struct SharedMemory shared_mem_new_client;
	InitShm(&(shared_mem_new_client.shm), &(shared_mem_new_client.ptr),
		SHM_NEW_CLIENT, O_RDWR, SIZE_BUFF_MSG);
	shared_mem_new_client.sem_read = sem_open(SEM_NEW_CLIENT_RD, 0);
	shared_mem_new_client.sem_write = sem_open(SEM_NEW_CLIENT_WR, 0);

	sem_wait(shared_mem_new_client.sem_write);
	sprintf(shared_mem_new_client.ptr, "%s", client->name);
	sem_post(shared_mem_new_client.sem_read);

	char answer[SIZE_BUFF_MSG];
	sem_wait(client->shared_mem_personal.sem_read);
	strcpy(answer, client->shared_mem_personal.ptr);
	sem_post(client->shared_mem_personal.sem_write);
	if (strcmp(answer, ANSWER_ERROR) == 0) {
		return;
	}

	int recv = GetDataFromServer(client);
	switch (recv) {
		case 1:
			printf(
			    "[X] GetDataFromServer: не удалось принять имена "
			    "других пользователей\n");
			return;
		case 2:
			printf(
			    "[X] GetDataFromServer: не удалось принять "
			    "сообщения");
			return;
	}

	InitNcursesStuff();
	InitWorkspace();
	PrintList(workspace.win_msgs, client->list_msgs);
	PrintList(workspace.win_clients, client->list_clients);

	pthread_t thread_messaging;
	pthread_t thread_broadcast;
	pthread_create(&thread_messaging, NULL, MessagingListener, client);
	pthread_create(&thread_broadcast, NULL, BroadcastListener, client);

	pthread_join(thread_messaging, NULL);
	pthread_join(thread_broadcast, NULL);
}
