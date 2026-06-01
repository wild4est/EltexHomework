#include <locale.h>
#include <signal.h>

#include "src/List.h"
#include "src/TUIFunc.h"
#include "src/ClientFunc.h"

void OnExit(int, void* args){
	endwin();
	struct Client* client = (struct Client*) args;
	SendMsg(client, EXIT_CODE);
	FreeClient(client);
	printf("Клиент завершил работу\n");
}

static void CloseClientHandler(){
	exit(EXIT_SUCCESS);
}

void InitSignalProcessing(){
        struct sigaction act;
	act.sa_handler = CloseClientHandler;
        sigaction(SIGINT, &act, NULL);
}

void main(int argc, char** argv){
	setlocale(LC_ALL, "ru_RU.UTF-8");
	if (argc < 5) {
		return;
	}

	int port_client = atoi(argv[1]);
	int port_server = atoi(argv[2]);

	char ip_addr_client[15];
	char ip_addr_server[15];

	strcpy(ip_addr_client, argv[3]);
	strcpy(ip_addr_server, argv[4]);

	static struct Client client;
	int recv = InitClient(&client, port_client, port_server, ip_addr_client, ip_addr_server);
	if ( recv == -1) {
		return;
	}

	InitSignalProcessing();	
	on_exit(OnExit, &client);

	struct Workspace workspace;
	InitWorkspace(&workspace, 22, 50, 1, 1);

	int recv_start_ws = StartWorkspace(&client, &workspace);
	if (recv_start_ws == -1) {
		printf("Сервер сообщил о завершении работы. Вас отключило\n");
	}	
}
