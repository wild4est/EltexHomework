#include "ServerStuff.h"

void main(){
	char* msg = "Hello!";
	
	sem_t* semaphore = sem_open(SEM_NAME, 0);

	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
        void* ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	printf("Получено сообщение: %s\n", (char*)ptr);
	printf("Отправлено сообщение: %s\n", msg);
	sprintf(ptr, "%s", msg);

	sem_post(semaphore);

	munmap(ptr, SIZE);
	close(shm_fd);
	sem_close(semaphore);
}
