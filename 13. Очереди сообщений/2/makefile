#CC=gcc
#
#all: server client
#
#
#server:
#	$(CC) server.c -o server 
#
#client:
#	$(CC) client.c -o client
#
#clean:
#	rm -f client server

#=====SERVER======

CC=gcc
CFLAGS=-c

SOURCE_SERV=$(wildcard serv/src/*.c)
OBJECTS_SERV=$(SOURCE_SERV:serv/src/%.c=serv/obj/%.o) serv/obj/server.o

all: objdir_clie objdir_serv server client

compile_server: objdir_serv server

objdir_serv:
ifeq ("$(wildcard serv/obj)","")
	mkdir serv/obj
endif


server: $(OBJECTS_SERV)
	$(CC) $^ -o server

serv/obj/%.o: serv/src/%.c
	$(CC) $(CFLAGS) $< -o $@

serv/obj/server.o: serv/server.c
	$(CC) $(CFLAGS) serv/server.c -o serv/obj/server.o

	
#=====CLIENT======

CC=gcc
CFLAGS=-c

SOURCE_CLIE=$(wildcard clie/src/*.c)
OBJECTS_CLIE=$(SOURCE_CLIE:clie/src/%.c=clie/obj/%.o) clie/obj/client.o

compile_client: objdir_client client

objdir_clie:
ifeq ("$(wildcard clie/obj)","")
	mkdir clie/obj
endif


client: $(OBJECTS_CLIE)
	$(CC) $^ -o client -lncursesw

clie/obj/%.o: clie/src/%.c
	$(CC) $(CFLAGS) $< -o $@

clie/obj/client.o: clie/client.c
	$(CC) $(CFLAGS) clie/client.c -o clie/obj/client.o 

#=====CLEAN======
clean:
	rm -rf client clie/obj/*.o clie/obj
	rm -rf server serv/obj/*.o serv/obj

