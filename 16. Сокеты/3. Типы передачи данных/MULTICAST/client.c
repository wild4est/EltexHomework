#include "ServerStuff.h"

void main(){
	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Не удалось создать файловый дескриптор\n");
		return;
	}

	struct sockaddr_in addr;
        addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(PORT);
	socklen_t len_addr = sizeof(addr);
       	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                printf("Не удалось забиндить порт %d\n", PORT);
                close(fd);
                return;
        }
	printf("Клиент забиндил порт %d\n", PORT);
	
	char buff[SIZE_BUFF];
	if (recvfrom(fd, buff, sizeof(buff), 0, (struct sockaddr *)&addr, &len_addr) < 0) {
		printf("Не удалось принять сообщение\n");
		close(fd);
		return;
	}
	printf("Клиент принял сообщение от сервера: %s\n", buff);

	close(fd);

}
