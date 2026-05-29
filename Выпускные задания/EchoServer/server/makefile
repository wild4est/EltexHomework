CC=gcc
CFLAGS=-c 

SOURCE=$(wildcard src/*.c)
OBJECTS=$(SOURCE:src/%.c=obj/%.o) obj/server_main.o

all: objdir server_main

objdir:
ifeq ("$(wildcard obj)","")
	mkdir obj
endif


server_main: $(OBJECTS)
	$(CC) $^ -o ../server_main

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

obj/server_main.o: server_main.c
	$(CC) $(CFLAGS) server_main.c -o obj/server_main.o

clean:
	rm -rf ../server_main obj/*.o obj
	rm -f run_test

run_test:
	$(CC) test/PointerFunc_test.c src/PointerFunc.c ../Unity/unity.c -o run_test
