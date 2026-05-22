#include "ServerStuff.h"

void main(){
	char* msg = "Hi!";
	char buff[SIZE_BUFF];
	
	key_t key = ftok(KEY_PATH, 200);
	int shm_fd = shmget(key, SIZE, IPC_CREAT | 0666);
	void* ptr = shmat(shm_fd, NULL, 0);

	key_t key_sem = ftok(KEY_PATH, 201);
	int sem_id = semget(key_sem, 1, IPC_CREAT | 0666);
	union semun arg;
	unsigned short sem_num = 0;
	arg.array = &sem_num;
	semctl(sem_id, 0, SETALL, arg);

	printf("Сервер отправил сообщение: %s\n", msg);
	sprintf(ptr, "%s", msg);
	
	struct sembuf sops;
	sops.sem_num = 0;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	semop(sem_id, &sops, 1);

	printf("Сервер принял сообщение от клиента: %s\n", (char*)ptr);

	shmctl(shm_fd, IPC_RMID, NULL);
	semctl(sem_id, 0, IPC_RMID);
}
