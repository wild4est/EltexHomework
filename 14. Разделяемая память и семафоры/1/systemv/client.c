#include "ServerStuff.h"

void main(){
	char* msg = "Hello!";
	char buff[SIZE_BUFF];
	mqd_t mqd_rd, mqd_wr;

	mqd_rd = mq_open(QUEUE_FSTC, O_RDONLY);
	mqd_wr = mq_open(QUEUE_FCTS, O_WRONLY);

	unsigned int prio; 
	mq_receive(mqd_rd, buff, sizeof(buff), &prio);
	printf("Клиент принял сообщение %s приоритетом %d\n", buff, prio);
	
	mq_send(mqd_wr, msg, sizeof(msg), 2);
	printf("Клиент отправил сообщение %s с приоритетом 2\n", msg);

	mq_close(mqd_rd);
	mq_close(mqd_wr);

}
