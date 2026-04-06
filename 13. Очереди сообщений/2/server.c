#include "ServerStuff.h"
#include <pthread.h>

#define MAX_COUNT_CLIENTS 10
#define MAX_COUNT_MSGS 10

struct Server{
	int client_count;
	int msg_count;
	char clients_name[MAX_COUNT_CLIENTS][SIZE_NAME_BUFF];
	char last_msgs[MAX_COUNT_MSGS][SIZE_MSG_BUFF];
	mqd_t queue_for_new_clients;
	mqd_t queue_for_messaging;
	mqd_t queue_for_broadcast;
	mqd_t queue_for_exit_clients;
};

void* NewClientListener(void* args){
	struct Server* serv = (struct Server*)args;
	
	char name[SIZE_NAME_BUFF];
	mq_receive(serv->queue_for_new_clients, name, sizeof(name), NULL);
	printf("Новый пользователь[ %s ] хочет присоединится к чату\n", name);
	
	if (client_count >= MAX_COUNT_CLIENTS) {
		printf("");
		return;
	}

	strcpy(serv->clients_name[client_count], name);
	serv->client_count++;
	printf("Пользователь[ %s ] присоединился к чату\n", name);
}

void* MessagingListener(void* args){
	struct Server* serv = (struct Server*)args;
	
	while(1){
		char msg[SIZE_MSG_BUFF];
		mq_recive(serv->queue_for_messaging, msg, sizeof(msg), NULL);
		printf("%s\n", msg);

		strcpy(serv->last_msgs[msg_count], msg);
		serv->msg_count++;

		for (int i=0; i<client_count; i++) {
			mq_send(serv->queue_for_broadcast, msg, sizeof(msg), 4);
		}
	}
}




void main(){
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = SIZE_MSG_BUFF;
	attr.mq_curmsgs = 0;

	struct Server serv;

	serv.client_count = 0;
	serv.msg_count = 0;
	serv.queue_for_new_clients = mq_open(QUEUE_NEW_CLIENT, O_RDONLY | O_CREAT, 0666, &attr);
	serv.queue_for_messaging = mq_open(QUEUE_MSG, O_RDONLY | O_CREAT, 0666, &attr);
	serv.queue_for_broadcast = mq_open(QUEUE_BROADCAST, O_WRONLY | O_CREAT, 0666, &attr);
	//serv.queue_for_exit_clients = mq_open(QUEUE_NEW_CLIENT, O_RDONLY | O_CREAT, 0666, &attr);

	pthread_t thread_for_new_clients;
	pthread_t thread_for_messaging;
	pthread_create(&thread_for_new_clients, NULL, NewClientListener, &serv);
	pthread_create(&thread_for_messaging, NULL, MessagingListener, &serv);
	pthread_join(thread_for_new_clients, NULL);
	pthread_join(thread_for_messaging, NULL);

	mq_close(serv.queue_for_new_clients);
	mq_close(serv.queue_for_messaging);
	mq_close(serv.queue_for_broadcast);
	
	mq_unlink(QUEUE_NEW_CLIENT);
	mq_unlink(QUEUE_MSG);
	mq_unlink(QUEUE_BROADCAST);
}
