#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void main(){
	char* msg = "Hi!";
	char* path_to_fifo = "fifo";
	mkfifo(path_to_fifo, 0666); //0666?
	int fd = open(path_to_fifo, O_WRONLY);
	write(fd, msg, strlen(msg)+1);
	printf("Сервер отправил сообщение: %s\n", msg);
	close(fd);
}
