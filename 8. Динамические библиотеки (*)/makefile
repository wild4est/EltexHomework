CC=gcc
CFLAGS=-c -Wall
CARCH=ar rc

#===================Сборка библиотеки=================
SOURCE_LIB=$(wildcard src/libcalc/*.c)
OBJECTS_LIB=$(SOURCE_LIB:src/libcalc/%.c=obj_lib/%.o)
PLUGINS=$(SOURCE_LIB:src/libcalc/%.c=plugins/lib%.so)

compile_lib: objdir_lib libdir libcalc 

objdir_lib:
ifeq ("$(wildcard obj_lib)","")
	mkdir obj_lib
endif

#libdir:
#ifeq ("$(wildcard lib)","")
#	mkdir lib
#endif

libdir:
ifeq ("$(wildcard plugins)","")
	mkdir plugins
endif

libcalc: $(OBJECTS_LIB) $(PLUGINS)

obj_lib/%.o: src/libcalc/%.c
	$(CC) $(CFLAGS) -fPIC $< -o $@

plugins/lib%.so: obj_lib/%.o
	$(CC) -shared $< -o $@

clean_lib:
	rm -rf obj_lib
	rm -rf plugins

#====================================================

#==================Сборка проекта====================

CLIB=-Llib -lcalc

SOURCE=src/UIfunc.c
OBJECTS=$(addprefix obj/, PluginsReader.o main.o)

compile_proj: objdir main

objdir:
ifeq ("$(wildcard obj)","")
	mkdir obj
endif

main: $(OBJECTS)
	$(CC) $^ -o main
#	$(CC) $^ $(CLIB) -o main

obj/PluginsReader.o: src/PluginsReader.c
	$(CC) $(CFLAGS) src/PluginsReader.c -o obj/PluginsReader.o

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
	rm -rf plugins
	rm -f run_test
#==================================================

#=================Сборка тестов====================

SO_FILES = $(addprefix -l, add mul div sub)

run_test: compile_lib
	$(CC) test/libcalc_test.c ../Unity/unity.c -Lplugins $(SO_FILES) -o run_test

clean_test:
	rm -f run_test

