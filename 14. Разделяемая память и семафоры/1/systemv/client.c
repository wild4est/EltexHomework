#include "ServerStuff.h"

void main(){
	char* msg = "Hello!";
	char buff[SIZE_BUFF];
	
	key_t key = ftok(KEY_PATH, 200);
        int shm_fd = shmget(key, SIZE, 0);
        void* ptr = shmat(shm_fd, NULL, 0);

        key_t key_sem = ftok(KEY_PATH, 201);
        int sem_id = semget(key_sem, 1, 0);

	printf("Клиент принял сообщение от сервера: %s\n", (char*)ptr);
	sprintf(ptr, "%s", msg);
	printf("Клиент отправил сообщение: %s\n", msg);

	struct sembuf sops;
        sops.sem_num = 0;
        sops.sem_op = 1;
        sops.sem_flg = 0;
        semop(sem_id, &sops, 1);

}
