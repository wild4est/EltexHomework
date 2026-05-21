#include "ServerStuff.h"

void main(){
	char* msg = "Hi!";

	sem_t* semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 0);

	int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	void* ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	printf("Отправлено сообщение: %s\n", msg);
	sprintf(ptr, "%s", msg);
	
	sem_wait(semaphore);

	printf("Получено сообщение: %s\n", (char*)ptr);
	
	munmap(ptr, SIZE);
	close(shm_fd);
	shm_unlink(SHM_NAME);
	sem_close(semaphore);
	sem_unlink(SEM_NAME);

}
