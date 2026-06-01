#include <pthread.h>
#include <locale.h>
#include <signal.h>

#include "src/ServerFunc.h"

void OnExit(int, void* args){
        struct Server* server = (struct Server*) args;
	FreeServer(server);	
	printf("Сервер завершил работу\n");
}

static void CloseClientHandler(){
        exit(EXIT_SUCCESS);
}

void InitSignalProcessing(){
        struct sigaction act;
        act.sa_handler = CloseClientHandler;
        sigaction(SIGINT, &act, NULL);
}

void main(int argc, char* argv[]){
	if (argc != 3) {
		printf("Для запуска сервера необходимо ввести порт и адрес\n");
		printf("./main_server 1234 0.0.0.0\n");
		return;
	}

	int port = atoi(argv[1]);
	char ip_addr[15];
	strcpy(ip_addr, argv[2]);
	static struct Server server;
	int recv = InitServer(&server, port, ip_addr);
	if ( recv == -1 ){
		printf("Что-то пошло не так\n");
		return;
	}

	InitSignalProcessing();
	on_exit(OnExit, &server);

	StartListeningMsg(&server);
	/*pthread_t thread_messaging;
	pthread_create(&thread_messaging, NULL, MessagingListener, &server);
	pthread_join(thread_messaging, NULL);
	*/	
}
