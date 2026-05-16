#include "ClientFunc.h"

int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize, int oflag){
        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = maxmsg;
        attr.mq_msgsize = msgsize;

        *mqd = mq_open(path, oflag | O_CREAT , 0666 , &attr);
	if (*mqd == (mqd_t)-1) {
	//	printf("[X] InitQueue: Не получилось инициализировать очередь\n");
	}
}

struct Client* InitClient(char* name){
	struct Client* client = malloc(sizeof(struct Client));
	client->name = name;
	
	char name_for_queue[SIZE_BUFF_NAME+1];
   	sprintf(name_for_queue, "/%s", name);

	//InitQueue(&(client->mqd_messaging), QUEUE_MESSAGING, MSG_COUNT, SIZE_BUFF_MSG, O_RDONLY);
	InitQueue(&(client->mqd_personal), name_for_queue, MSG_COUNT, SIZE_BUFF_MSG, O_RDONLY);
	//InitQueue(&(client->mqd_exit), QUEUE_EXIT, MSG_COUNT, SIZE_BUFF_MSG, O_RDONLY);	
	client->mqd_messaging = mq_open(QUEUE_MESSAGING, O_WRONLY);
	client->mqd_exit = mq_open(QUEUE_EXIT, O_WRONLY);


	client->list_clients = InitList();
	client->list_msgs = InitList();

	return client;
}


int GetDataFromServer(struct Client* client){
	ssize_t recv;
	//printf("Пользователи на сервере:\n");
	char get_name[SIZE_BUFF_MSG];
	recv = mq_receive(client->mqd_personal, get_name, sizeof(get_name), NULL);
	if (recv == -1) {
		//printf("[X] GetDataFromServer: Не удалось принять сообщение 1\n");
		return 1;
	}
	while(strcmp(get_name, ANSWER_END) != 0){
		AddNode(client->list_clients, get_name);
		//printf("%s\n", get_name);
		
		ssize_t recv = mq_receive(client->mqd_personal, get_name, sizeof(get_name), NULL);
		if (recv == -1) {
			//printf("[X] GetDataFromServer: Не удалось принять сообщение 2\n");
			return 1;
		}
	}

	//printf("\nПоследние сообщения:\n");
	char get_msg[SIZE_BUFF_MSG];
	recv = mq_receive(client->mqd_personal, get_msg, sizeof(get_msg), NULL);
	if (recv == -1) {
		//printf("[X] GetDataFromServer: Не удалось принять сообщение 3\n");
		return 2;
	}
	while(strcmp(get_msg, ANSWER_END) != 0){
		AddNode(client->list_msgs, get_msg);
		//printf("%s\n", get_msg);
	
		ssize_t recv = mq_receive(client->mqd_personal, get_msg, sizeof(get_msg), NULL);
		if (recv == -1) {
			//printf("[X] GetDataFromServer: Не удалось принять сообщение 4\n");
			return 2;
		}
	}

	return 0;
}

int SendMsgToServer(mqd_t mqd_messaging, char* name, char* text_msg){
	char msg[SIZE_BUFF_MSG];
	sprintf(msg, "%s: %s", name, text_msg);
	int recv = mq_send(mqd_messaging, msg, strlen(msg)+1, 1);
	if (recv == -1) {
		//printf("[X] MessagingListener: Не удалось отправить сообщение\n");
		return 1;
	}

	return 0;
		
	//printf("Отправлено сообщение:\n{ %s }\n", msg);
}

int GetMsgFromServer(mqd_t mqd_personal, struct List** list_msgs, struct List** list_clients){
	char msg[SIZE_BUFF_MSG];
	ssize_t recv = mq_receive(mqd_personal, msg, sizeof(msg), NULL);
	if (recv == -1) {
		//int err = errno;
	//	printf("[X] BroadcastListener: Не удалось принять сообщение\n");
	//	printf("Error = %d\n", err);
		return 1;
	}
	//printf("Получено сообщение:\n{ %s }\n", msg);
	if (msg[0] == '+') {
		memmove(msg, msg+1, strlen(msg));
		AddNode(*list_clients, msg);
	}else if(msg[0] == '-') {
		memmove(msg, msg+1, strlen(msg));
		DeleteNode(*list_clients, msg);
	}else{
		if ((*list_msgs)->length >= MSG_COUNT) {
			DeleteNode(*list_msgs, (*list_msgs)->start->msg);
		}
		AddNode(*list_msgs, msg);
	}

	return 0;	
}

