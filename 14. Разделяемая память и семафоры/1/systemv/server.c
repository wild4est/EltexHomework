#include "ServerStuff.h"

void main(){
	char* msg = "Hi!";
	char buff[SIZE_BUFF];

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 2;
	attr.mq_msgsize = 10;
	attr.mq_curmsgs = 0;

	mqd_t mqd_rd, mqd_wr;
	
	mqd_rd = mq_open(QUEUE_FCTS, O_RDONLY | O_CREAT, 0666, &attr);
	mqd_wr = mq_open(QUEUE_FSTC, O_WRONLY | O_CREAT, 0666, &attr);

	mq_send(mqd_wr, msg, sizeof(msg), 1);
	printf("Сервер отправил сообщение %s с приоритетом 1\n", msg);

	unsigned int prio;
	mq_receive(mqd_rd, buff, sizeof(buff), &prio);
	printf("Сервер принял сообщение %s приоритетом %d\n", buff, prio);
	mq_close(mqd_rd);
	mq_close(mqd_wr);

	mq_unlink(QUEUE_FCTS);
	mq_unlink(QUEUE_FSTC);
	
}
