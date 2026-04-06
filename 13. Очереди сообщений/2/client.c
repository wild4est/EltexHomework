#include "ServerStuff.h"

struct Client{
	int msg_count;
	char last_msgs[MAX_COUNT_MSGS][SIZE_MSG_BUFF];
	char name[SIZE_NAME_BUFF];
};


void* MessagingListener(void* args){
	struct Client* client = (struct Client*)args;
	
	
}

void main(int argc, char* argv[]){
	if (argc != 2) {
		return;
	}
	
	struct Client client;
	strcpy(client.name, argv[1]);
	mqd_t queue_for_new_clients = mq_open(QUEUE_NEW_CLIENT, O_WRONLY);
       	mq_send(queue_for_new_clients, client.name, sizeof(client.name), 2);
	mq_close(queue_for_new_clients);

	mqd_t queue_for_messaging = mq_open(QUEUE_MSG, O_WRONLY);
	mqd_t queue_for_broadcast = mq_open(QUEUE_BROADCAST, O_RDONLY);


}
