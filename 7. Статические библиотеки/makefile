CC=gcc
CFLAGS=-c -Wall
CARCH=ar rc

#===================Сборка библиотеки=================
SOURCE_LIB=$(wildcard src/libcalc/*.c)
OBJECTS_LIB=$(SOURCE_LIB:src/libcalc/%.c=obj_lib/%.o)

compile_lib: objdir_lib libdir libcalc 

objdir_lib:
ifeq ("$(wildcard obj_lib)","")
	mkdir obj_lib
endif

libdir:
ifeq ("$(wildcard lib)","")
	mkdir lib
endif

libcalc: $(OBJECTS_LIB)
	$(CARCH) lib/libcalc.a $^

obj_lib/%.o: src/libcalc/%.c
	$(CC) $(CFLAGS) $< -o $@

clean_lib:
	rm -rf obj_lib

#====================================================

#==================Сборка проекта====================

CLIB=-Llib -lcalc

SOURCE=src/UIfunc.c
OBJECTS=$(addprefix obj/, UIfunc.o main.o)

compile_proj: objdir main

objdir:
ifeq ("$(wildcard obj)","")
	mkdir obj
endif

main: $(OBJECTS)
	$(CC) $^ $(CLIB) -o main

obj/UIfunc.o: src/UIfunc.c
	$(CC) $(CFLAGS) src/UIfunc.c -o obj/UIfunc.o

obj/main.o: main.c
	$(CC) $(CFLAGS) main.c -o obj/main.o

clean_proj:
	rm -rf obj

#===================================================

#============Сборка библиотеки и проекта============

all: compile_lib compile_proj

clean:
	rm -rf lib
	rm -rf obj_lib
	rm -rf obj main

#==================================================

#=================Сборка тестов====================

run_test: compile_lib
	$(CC) test/libcalc_test.c ../Unity/unity.c -Llib -lcalc -o run_test

clean_test:
	rm -f run_test
