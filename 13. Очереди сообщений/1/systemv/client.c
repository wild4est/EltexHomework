#include "ServerStuff.h"

void main(){
	char* msg = "Hello!";
	int qid;
	key_t queue_key = ftok(NAME_QUEUE, PROJ_ID);
	qid = msgget(queue_key, IPC_CREAT | 0666);

	struct Msg_t msg_rcv;
	msgrcv(qid, &msg_rcv, sizeof(msg_rcv), 1, 0);
	printf("Клиент принял сообщение %s с приоритетом %ld\n", msg_rcv.msg, msg_rcv.mtype);

	struct Msg_t msg_snd;
	msg_snd.mtype = 2;
	strcpy(msg_snd.msg, msg);
	msgsnd(qid, &msg_snd, sizeof(msg), 0);
	printf("Клиент отправил сообщение: %s с приоритетом %ld\n", msg_snd.msg, msg_snd.mtype);
}
