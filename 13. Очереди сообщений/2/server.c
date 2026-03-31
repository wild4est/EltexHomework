#include "ServerStuff.h"

#define MAX_COUNT_CLIENTS

struct Server{
	int count_clients;
	int count_msgs;
	char** clients;
	char** msgs;
	mqd_t q_read;
	mqd_t q_write;
};

/*
void KeyListener(){
	
	
}*/

void ClinetListener(struct Server* serv){
	while (1) {
		char client_name[SIZE_NAME];
		mq_recive(serv->q_read, client_name, sizeof(client_name), NULL);
		
		if (count_clients == MAX_COUNT_CIENTS) {
			continue;
		}

		serv->clients[serv->count_clients] = malloc(sizeof(char) * SIZE_NAME);
		strcopy(serv->clients[serv->count_clients], client_name);
		serv->count_clients++;
		
		
		
	}
}


void main(){
	struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = 10;
        attr.mq_curmsgs = 0;

        //mqd_t mqd_rd, mqd_wr;
    
	struct Server serv;
	serv.count_clients = 0;
	serv.count_msga = 0;
	serv.clients = malloc(sizeof(char*) * MAX_COUNT_CLIENTS);
	serv.msgs = malloc(sizeof(char*) * MAX_COUNT_CLIENTS);
        serv.q_read = mq_open(QUEUE_FCTS, O_RDONLY | O_CREAT, 0666, &attr);
        serv.q_write = mq_open(QUEUE_FSTC, O_WRONLY | O_CREAT, 0666, &attr);
	
	printf("Сервер запустился и ждёт клиентов.\n");

}
