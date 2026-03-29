#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE_BUFF 10

void main(){
	char buff[SIZE_BUFF];
	char* path_to_fifo = "fifo";
	int fd = open(path_to_fifo, O_RDONLY);
	read(fd, buff, sizeof(buff));
	printf("Клиент получил сообщение: %s\n", buff);
	close(fd);
	unlink(path_to_fifo);
}
