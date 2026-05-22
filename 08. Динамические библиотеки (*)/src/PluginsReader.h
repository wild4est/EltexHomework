#include <dirent.h>
#include <malloc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#define OPERATIONLIB_MAX 10
#define EXIT 1
#define SIZE_OF_NAME 10

struct OperationLib{
	void* handle;	
	int (*func_ptr)(int*, int, int);
	char oper_name[SIZE_OF_NAME];
};

//int OPERATIONLIB_SIZE = 0;

void AlternateScreenOn();
void AlternateScreenOff();
void Clear();
int GetFirstNChar(char* output, char* input, int n);
int ReadPlugins(struct OperationLib** operation_libs, int* n);
void PrintMenu(struct OperationLib* operation_libs, int n);
int UseOperation(int (*func_ptr)(int*, int, int));
int Output();
