#include <ctype.h>
#include <locale.h>
#include <pthread.h>
#include <signal.h>

#include "src/ServerFunc.h"

void OnExit(int, void* args) {
	struct Server* server = (struct Server*)args;
	FreeServer(server);
	printf("Сервер завершил работу\n");
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

void main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Для запуска сервера необходимо ввести порт и адрес\n");
		printf(" ./server_main <порт сервера> <адрес сервера>\n");
		printf("Пример: sudo ./server_main 1234 0.0.0.0\n");
		return;
	}

	if (!StringIsNumberCheck(argv[1], strlen(argv[1]))) {
		printf("Указанный порт не является коректным\n");
		return;
	}

	int port = atoi(argv[1]);
	char ip_addr[15];
	strcpy(ip_addr, argv[2]);
	static struct Server server;
	int recv = InitServer(&server, port, ip_addr);
	if (recv == -1) {
		printf("Что-то пошло не так\n");
		return;
	}

	InitSignalProcessing();
	on_exit(OnExit, &server);

	StartListeningMsg(&server);
}
