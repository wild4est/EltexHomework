#include "ServerFunc.h"

int InitQueue(mqd_t* mqd, const char* path, long maxmsg, long msgsize, int oflag){
        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = maxmsg;
        attr.mq_msgsize = msgsize;

        *mqd = mq_open(path, oflag | O_CREAT , 0666 , &attr);
	if (*mqd == (mqd_t)-1) {
		int err = errno;
		printf("[X] InitQueue: Не получилось создать очередь\n");
		printf("%d\n", err);
	}
}

struct Server* InitServer(){
        //serv->client_count = 0;
        //serv->msg_count = 0;
        struct Server* serv = malloc(sizeof(struct Server));
	serv->list_clients = InitListClient();
        serv->list_msgs = InitListMsg();
        InitQueue(&(serv->mqd_new_clients), QUEUE_NEW_CLIENT, MAX_COUNT_MSGS, SIZE_BUFF_NAME, O_RDONLY);
        InitQueue(&(serv->mqd_messaging), QUEUE_MESSAGING, MAX_COUNT_MSGS, SIZE_BUFF_MSG, O_RDONLY);
        InitQueue(&(serv->mqd_exit), QUEUE_EXIT, MAX_COUNT_MSGS, SIZE_BUFF_NAME, O_RDONLY);

	printf("[!] Сервер поднят\n");
	printf("Для выхода нажмите CTR + C\n");
	printf("==============================================\n");
	return serv;
}

int OpenQueueToClient(char* name, mqd_t* mqd_client){
	char name_for_queue[SIZE_BUFF_NAME+1];
        sprintf(name_for_queue, "/%s", name);
        *mqd_client = mq_open(name_for_queue, O_WRONLY);

	if (*mqd_client == (mqd_t)-1) {
		printf("[X] Сервер не смог открыть очередь %s\n", name_for_queue);
		return 1;
	}

	return 0;
}

void SendMsgToClient(mqd_t mqd_client, char* answer){
        int recv = mq_send(mqd_client, answer, strlen(answer)+1, 1);
	if (recv == -1) {
		int err = errno;
		printf("[X] SendMsgToClient: Не удалось отправить сообщение\n");
		printf("Error %d\n", err);
	}
}

void SendMsgToAllClients(struct ListClient* list, char* answer){
	struct NodeClient* node = list->start;
	while(node != NULL){
		SendMsgToClient(node->mqd_personal, answer);	
		node = node->next;
	}
}

void SendDataToClient(mqd_t mqd_client, struct ListClient* list_client, struct ListMsg* list_msg){
	int recv;
	char end_msg[SIZE_BUFF_NAME];
	strcpy(end_msg, ANSWER_END);
	struct NodeClient* node_client = list_client->start;
	while(node_client != NULL) {
		SendMsgToClient(mqd_client, node_client->name);	
		node_client = node_client->next; 
	}
	SendMsgToClient(mqd_client, end_msg);

	struct NodeMsg* node_msg = list_msg->start;
	while(node_msg != NULL){
		SendMsgToClient(mqd_client, node_msg->msg);	
		node_msg = node_msg->next; 
	}
	SendMsgToClient(mqd_client, end_msg);
}

void* NewClientListener(void* args){
        struct Server* serv = (struct Server*)args;

        while (1) {
                char name[SIZE_BUFF_NAME];
                mq_receive(serv->mqd_new_clients, name, sizeof(name), NULL);
                printf("[!] Новый пользователь[%s] хочет присоединится к чату\n", name);

		mqd_t mqd_client;
		OpenQueueToClient(name, &mqd_client);

                if (serv->list_clients->length >= MAX_COUNT_CLIENTS) {
                        printf("[X] Пользователь[%s] не был принят, так как сервер переполнен\n", name);
                        SendMsgToClient(mqd_client, ANSWER_ERROR);
                        continue;
                }

		char msg[SIZE_BUFF_NAME+1];
		sprintf(msg, "+%s", name);
		SendMsgToAllClients(serv->list_clients, msg);

                AddNodeClient(serv->list_clients, name, mqd_client);
                SendMsgToClient(mqd_client, ANSWER_GOOD);
                SendDataToClient(mqd_client, serv->list_clients, serv->list_msgs);
		
		printf("[O] Новый пользователь[%s] принят\n", name);
                printf("Текущее количество клиентов %d/%d\n", serv->list_clients->length, MAX_COUNT_CLIENTS);
        }
}

void* MessagingListener(void* args){
        struct Server* serv = (struct Server*)args;

        while(1){
                char msg[SIZE_BUFF_MSG];
                ssize_t recv_r = mq_receive(serv->mqd_messaging, msg, sizeof(msg), NULL);
                if (recv_r == -1) {
			printf("[X] MessagingListener: Не удалось принять сообщение\n");
			continue;
		}
		printf("[!] Поступило новое сообщение:\n{ %s }\n", msg);

        	if (serv->list_msgs->length >= MAX_COUNT_MSGS) {
			DeleteNodeMsg(serv->list_msgs, serv->list_msgs->start->msg);
		}
		AddNodeMsg(serv->list_msgs, msg); 
		
		struct NodeClient* cur_client = serv->list_clients->start;
		while (cur_client != NULL) {
			SendMsgToClient(cur_client->mqd_personal, msg);
			cur_client = cur_client->next;
		}
        }
}

void* ExitListener(void* args){
	struct Server* serv = (struct Server*)args;

	while(1){
		char name[SIZE_BUFF_NAME];
                ssize_t recv_r = mq_receive(serv->mqd_exit, name, sizeof(name), NULL);
                if (recv_r == -1) {
			printf("[X] ExitListener: Не удалось принять сообщение\n");
			continue;
		}

		DeleteNodeClient(serv->list_clients, name);
		char msg[SIZE_BUFF_NAME+1];
		sprintf(msg, "-%s", name);

		SendMsgToAllClients(serv->list_clients, msg);
		printf("[!] Пользователь[%s] покинул чат\n", name);

	}
}
