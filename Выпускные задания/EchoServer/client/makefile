CC=gcc
CFLAGS=-c 

SOURCE=$(wildcard src/*.c)
OBJECTS=$(SOURCE:src/%.c=obj/%.o) obj/client_main.o

all: objdir client_main

objdir:
ifeq ("$(wildcard obj)","")
	mkdir obj
endif


client_main: $(OBJECTS)
	$(CC) $^ -o ../client_main -lncursesw

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

obj/client_main.o: client_main.c
	$(CC) $(CFLAGS) client_main.c -o obj/client_main.o

clean:
	rm -rf ../client_main obj/*.o obj

