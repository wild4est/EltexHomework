#include <ctype.h>
#include <locale.h>
#include <signal.h>

#include "src/ClientFunc.h"
#include "src/List.h"
#include "src/TUIFunc.h"

void OnExit(int, void* args) {
	endwin();
	struct Client* client = (struct Client*)args;
	SendMsg(client, EXIT_CODE);
	FreeClient(client);
	printf("Клиент завершил работу\n");
}

static void CloseClientHandler() { exit(EXIT_SUCCESS); }

void InitSignalProcessing() {
	struct sigaction act;
	act.sa_handler = CloseClientHandler;
	sigaction(SIGINT, &act, NULL);
}

int StringIsNumberCheck(char* str, size_t len) {
	for (int i = 0; i < len; i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}
	return 1;
}

void main(int argc, char** argv) {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	if (argc < 5) {
		printf(
		    "Во время запуска клиента ему необходимо задать порт и "
		    "сетевой адрес, а так же указать порт и адрес сервера\n");
		printf(
		    " ./client_main <порт клиента> <порт сервера> <сетевой "
		    "адрес клиента> <сетевой адрес сервера>\n");
		printf(
		    "Пример: sudo ./client_main 1234 1234 0.0.0.0 0.0.0.0\n");
		return;
	}

	if (!StringIsNumberCheck(argv[1], strlen(argv[1])) ||
	    !StringIsNumberCheck(argv[2], strlen(argv[2]))) {
		printf("Один из указанных портов не является коректным\n");
		return;
	}

	int port_client = atoi(argv[1]);
	int port_server = atoi(argv[2]);

	char ip_addr_client[15];
	char ip_addr_server[15];

	strcpy(ip_addr_client, argv[3]);
	strcpy(ip_addr_server, argv[4]);

	static struct Client client;
	int recv = InitClient(&client, port_client, port_server, ip_addr_client,
			      ip_addr_server);
	if (recv == -1) {
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
