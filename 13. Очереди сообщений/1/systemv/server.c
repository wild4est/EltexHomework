#include "ServerStuff.h"

void main(){
	char* msg = "Hi!";
	int qid;
	key_t queue_key = ftok(NAME_QUEUE, PROJ_ID);
	qid = msgget(queue_key, IPC_CREAT | 0666);

	struct Msg_t msg_snd;
	msg_snd.mtype = 1;
	strcpy(msg_snd.msg, msg);	
	msgsnd(qid, &msg_snd, sizeof(msg_snd), 0);
	printf("Сервер отправил сообщение %s с приоритетом %ld\n", msg_snd.msg, msg_snd.mtype);

	struct Msg_t msg_rcv;
	msgrcv(qid, &msg_rcv, sizeof(msg_rcv), 2, 0);
	printf("Сервер принял сообщение %s с приоритетом %ld\n", msg_rcv.msg, msg_rcv.mtype);

	msgctl(qid, IPC_RMID, NULL);
	printf("Сервер завершил работу\n");
}
