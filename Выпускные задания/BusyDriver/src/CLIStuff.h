#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DriverManager.h"

#define GETSTR_SIZE_BUFF 255
#define WORDS_COUNT 5

#define COL_COM 32
#define COL_VAL 33
#define COL_BD 31

void DeleteEnter(char*);
int WordIsNumberCheck(char* str, int len);
void Split(char*** words, int* count, char* str, const char* restrict sym);
void PrintCommands();
void PrintCar();
void PrintGreeting();
void MainCLI();
