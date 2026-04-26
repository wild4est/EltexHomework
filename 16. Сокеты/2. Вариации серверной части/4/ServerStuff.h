#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>

#define SOCKET_PATH_TCP "/tmp/test_socket_tcp"
#define SOCKET_PATH_UDP "/tmp/test_socket_udp"
#define SIZE_BUFF 50
#define ERROR_CODE "Error"
