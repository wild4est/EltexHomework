CC=gcc
CFLAGS=-c 

SOURCE=$(wildcard src/*.c)
OBJECTS=$(SOURCE:src/%.cpp=obj/%.o) obj/main.o

all: objdir main

objdir:
ifeq ("$(wildcard obj)","")
	mkdir obj
endif


main: $(OBJECTS)
	$(CC) $^ -o main

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

obj/main.o: main.c
	$(CC) $(CFLAGS) main.c -o obj/main.o

clean:
	rm -rf main obj/*.o obj
	rm -f run_test

run_test:
	$(CC) test/ArrayFunc_test.c src/ArrayFunc.c ../Unity/unity.c -o run_test
