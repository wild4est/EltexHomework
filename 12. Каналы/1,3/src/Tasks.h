#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define SIZE_BUFF 5
#define SIZE_COMBUFF 256
#define SIZE_COMMANDS 10

int task1();
void DeleteEnter(char* str);
char** Split(char* ch, char* combuff, int* n);
void FreeStrArr(char** arr);
void task3();
