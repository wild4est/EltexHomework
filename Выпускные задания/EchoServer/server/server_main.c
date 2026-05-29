#include <pthread.h>
#include "src/ServerFunc.h"

void main(int argc, char* argv[]){
	if (argc != 2) {
		printf("Для запуска сервера необходимо ввести порт\n");
		printf("./main_server <PORT>\n");
		return;
	}

	int port = atoi(argv[1]);

	struct Server server;
	int recv = InitServer(&server, port);
	if ( recv == -1 ){
		printf("Что-то пошло не так\n");
		return;
	}

	StartListeningMsg(&server);
	/*pthread_t thread_messaging;
	pthread_create(&thread_messaging, NULL, MessagingListener, &server);
	pthread_join(thread_messaging, NULL);
	*/	
}
