/*#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE_BUFF 20
#define PORT 49134
#define IP_ADDR "127.0.0.1"
*/

#include "src/TUIFunc.h"
#include "src/ClientFunc.h"

void main(int argc, char** argv){
	if (argc < 5) {
		printf("Не хаватае адресов и/или портов формата <client_addr> <server_addr> <client_port> <server_port>\n");
		printf("Пример: sudo ./client_main 12345 12345 0.0.0.0 0.0.0.0\n");
		return;
	}
	
	int client_port = atoi(argv[1]);
	int server_port = atoi(argv[2]);
	
	char client_addr[15];
	char server_addr[15];
	
	strcpy(client_addr, argv[3]);
	strcpy(server_addr, argv[4]);
	
	printf("%s\n", client_addr);
	printf("%s\n", server_addr);

	struct Client client;
	int recv_init =  InitClient(&client, client_port, server_port, client_addr, server_addr);
	if (recv_init == -1) {
		printf("Есть вероятность, что вы запустили без sudo\n");
		return;
	}

	struct Workspace workspace;
	InitWorkspace(&workspace, 22, 30, 1, 1);

	StartWorkspace(&client, &workspace);

}
