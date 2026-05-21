#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#define SIZE 256
#define SEM_NAME "/sem_test"
#define SHM_NAME "/name_for_shm"
#define SIZE_BUFF 10

