#include "ServerFunc.h"

int InitShm(int* shm_fd, void** ptr, const char* name, size_t size) {
	*shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (*shm_fd == -1) {
		int err = errno;
		printf("Error[ InitShm ]= %d\n", err);
	}

	int recv = ftruncate(*shm_fd, size);
	if (recv == -1) {
		int err = errno;
		printf("Error[ InitShm ]= %d\n", err);
	}

	*ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, *shm_fd, 0);
	if (*ptr == (void*)-1) {
		int err = errno;
		printf("Error[ InitShm ]= %d\n", err);
	}

	return 0;
}

struct Server* InitServer() {
	struct Server* serv = malloc(sizeof(struct Server));
	serv->list_clients = InitListClient();
	serv->list_msgs = InitListMsg();

	serv->shared_mem_new_client.sem_read =
	    sem_open(SEM_NEW_CLIENT_RD, O_CREAT, 0666, 0);
	serv->shared_mem_messaging.sem_read =
	    sem_open(SEM_MESSAGING_RD, O_CREAT, 0666, 0);
	serv->shared_mem_exit.sem_read =
	    sem_open(SEM_EXIT_RD, O_CREAT, 0666, 0);

	serv->shared_mem_new_client.sem_write =
	    sem_open(SEM_NEW_CLIENT_WR, O_CREAT, 0666, 1);
	serv->shared_mem_messaging.sem_write =
	    sem_open(SEM_MESSAGING_WR, O_CREAT, 0666, 1);
	serv->shared_mem_exit.sem_write =
	    sem_open(SEM_EXIT_WR, O_CREAT, 0666, 1);

	InitShm(&(serv->shared_mem_new_client.shm),
		&(serv->shared_mem_new_client.ptr), SHM_NEW_CLIENT,
		SIZE_BUFF_NAME + 1);
	InitShm(&(serv->shared_mem_messaging.shm),
		&(serv->shared_mem_messaging.ptr), SHM_MESSAGING,
		SIZE_BUFF_NAME + 1);
	InitShm(&(serv->shared_mem_exit.shm), &(serv->shared_mem_exit.ptr),
		SHM_EXIT, SIZE_BUFF_NAME + 1);

	printf("[!] Сервер поднят\n");
	printf("Для выхода нажмите CTR + C\n");
	printf("==============================================\n");
	return serv;
}

int OpenQueueToClient(char* name, struct SharedMemory* shared_mem) {
	char name_for_shm[SIZE_BUFF_NAME + 1];
	sprintf(name_for_shm, "/%s", name);
	shared_mem->shm = shm_open(name_for_shm, O_RDWR, 0666);
	if (shared_mem->shm == -1) {
		return 1;
	}

	ftruncate(shared_mem->shm, SIZE_BUFF_MSG);
	shared_mem->ptr =
	    mmap(0, SIZE_BUFF_MSG, PROT_WRITE, MAP_SHARED, shared_mem->shm, 0);

	char name_for_sem[SIZE_BUFF_NAME + 4];
	sprintf(name_for_sem, "/rd_%s", name);
	shared_mem->sem_read = sem_open(name_for_sem, 0);
	sprintf(name_for_sem, "/wr_%s", name);
	shared_mem->sem_write = sem_open(name_for_sem, 0);

	return 0;
}

void SendMsgToClient(struct SharedMemory* shared_mem, char* answer) {
	sem_wait(shared_mem->sem_write);
	sprintf(shared_mem->ptr, "%s", answer);
	sem_post(shared_mem->sem_read);
}

void SendMsgToAllClients(struct ListClient* list, char* answer) {
	struct NodeClient* node = list->start;
	while (node != NULL) {
		SendMsgToClient(&(node->shared_mem_per), answer);
		node = node->next;
	}
}

void SendDataToClient(struct SharedMemory shared_mem,
		      struct ListClient* list_client,
		      struct ListMsg* list_msg) {
	int recv;
	char end_msg[SIZE_BUFF_NAME];
	strcpy(end_msg, ANSWER_END);
	struct NodeClient* node_client = list_client->start;
	while (node_client != NULL) {
		SendMsgToClient(&shared_mem, node_client->name);
		node_client = node_client->next;
	}
	SendMsgToClient(&shared_mem, end_msg);

	struct NodeMsg* node_msg = list_msg->start;
	while (node_msg != NULL) {
		SendMsgToClient(&shared_mem, node_msg->msg);
		node_msg = node_msg->next;
	}
	SendMsgToClient(&shared_mem, end_msg);
}

void* NewClientListener(void* args) {
	struct Server* serv = (struct Server*)args;

	while (1) {
		sem_wait(serv->shared_mem_new_client.sem_read);
		char name[SIZE_BUFF_NAME];
		strcpy(name, serv->shared_mem_new_client.ptr);
		sem_post(serv->shared_mem_new_client.sem_write);
		printf(
		    "[!] Новый пользователь[%s] хочет присоединится к чату\n",
		    name);

		struct SharedMemory shared_mem_client;
		OpenQueueToClient(name, &shared_mem_client);
		if (serv->list_clients->length >= MAX_COUNT_CLIENTS) {
			printf(
			    "[X] Пользователь[%s] не был принят, так как "
			    "сервер переполнен\n",
			    name);
			SendMsgToClient(&shared_mem_client, ANSWER_ERROR);
			continue;
		}

		char msg[SIZE_BUFF_NAME + 1];
		sprintf(msg, "+%s", name);
		SendMsgToAllClients(serv->list_clients, msg);

		AddNodeClient(serv->list_clients, name, shared_mem_client);
		SendMsgToClient(&shared_mem_client, ANSWER_GOOD);  //!!
		SendDataToClient(shared_mem_client, serv->list_clients,
				 serv->list_msgs);

		printf("[O] Новый пользователь[%s] принят\n", name);
		printf("Текущее количество клиентов %d/%d\n",
		       serv->list_clients->length, MAX_COUNT_CLIENTS);
	}
}

void* MessagingListener(void* args) {
	struct Server* serv = (struct Server*)args;

	while (1) {
		sem_wait(serv->shared_mem_messaging.sem_read);
		char msg[SIZE_BUFF_MSG];
		strcpy(msg, serv->shared_mem_messaging.ptr);
		sem_post(serv->shared_mem_messaging.sem_write);
		printf("[!] Поступило новое сообщение:\n{ %s }\n", msg);

		if (serv->list_msgs->length >= MAX_COUNT_MSGS) {
			DeleteNodeMsg(serv->list_msgs,
				      serv->list_msgs->start->msg);
		}
		AddNodeMsg(serv->list_msgs, msg);

		struct NodeClient* cur_client = serv->list_clients->start;
		while (cur_client != NULL) {
			SendMsgToClient(&(cur_client->shared_mem_per), msg);
			cur_client = cur_client->next;
		}
	}
}

void* ExitListener(void* args) {
	struct Server* serv = (struct Server*)args;

	while (1) {
		sem_wait(serv->shared_mem_exit.sem_read);
		char name[SIZE_BUFF_NAME];
		strcpy(name, serv->shared_mem_exit.ptr);
		sem_post(serv->shared_mem_exit.sem_write);

		DeleteNodeClient(serv->list_clients, name);
		char msg[SIZE_BUFF_NAME + 1];
		sprintf(msg, "-%s", name);

		SendMsgToAllClients(serv->list_clients, msg);
		printf("[!] Пользователь[%s] покинул чат\n", name);
	}
}
