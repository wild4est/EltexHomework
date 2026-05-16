#include <signal.h>
#include <locale.h>
#include "src/ClientFunc.h"
#include "src/UIFunc.h"

extern struct Workspace workspace;
struct Client* client = NULL;

static void CloseClientHandler(){
	if (client == NULL) {
		//printf("[X] CloseClientHandler: структура клиента равна NULL\n");
		return;
	}
	
	mqd_t mqd_exit = mq_open(QUEUE_EXIT, O_WRONLY);
	mq_send(mqd_exit, client->name, strlen(client->name)+1, 1);
	mq_close(mqd_exit);
	
	mq_close(client->mqd_personal);
	char name_for_queue[SIZE_BUFF_NAME+1];
	sprintf(name_for_queue, "/%s", client->name);
	mq_unlink(name_for_queue);

	FreeList(client->list_clients);
	FreeList(client->list_msgs);
	endwin();
	exit(EXIT_SUCCESS);
}

void InitSignalProcessing(){
        struct sigaction act;
	act.sa_handler = CloseClientHandler;
        sigaction(SIGINT, &act, NULL);
}

void main(int argc, char* argv[]){
	setlocale(LC_ALL, "ru_RU.UTF-8");
	if (argc < 2) {
		printf("[X] Ошибка: нету имени клиента\n");
		return;
	}

	char* name = argv[1];
	client = InitClient(name);
	InitSignalProcessing();

	mqd_t mqd_new_client = mq_open(QUEUE_NEW_CLIENT, O_WRONLY);
	
	mq_send(mqd_new_client, client->name, strlen(client->name)+1, 1);

	//printf("Пользователь[%s] хочет подключиться к серверу\n", name);
	
	char answer[SIZE_BUFF_MSG];
	mq_receive(client->mqd_personal, answer, sizeof(answer), NULL);
	//printf("Answer: %s\n", answer);
	if (strcmp(answer, ANSWER_ERROR) == 0) {
		return;
	}

 	int recv = GetDataFromServer(client);		
	switch(recv) {
		case 1:
			printf("[X] GetDataFromServer: не удалось принять имена других пользователей\n");
			return;
		case 2:
			printf("[X] GetDataFromServer: не удалось принять сообщения");
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


//	endwin();
}
