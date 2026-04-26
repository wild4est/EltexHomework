CC=gcc

all: server client_tcp client_udp

server:
	$(CC) server.c -o server 

client_tcp:
	$(CC) client_tcp.c -o client_tcp

client_udp:
	$(CC) client_udp.c -o client_udp

clean:
	rm -f client_tcp client_udp server
