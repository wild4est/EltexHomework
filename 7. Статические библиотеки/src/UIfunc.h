#include <stdio.h>
#include "libcalc/libcalc.h"

enum condition{
	ADD=1,
	SUB,
	MUL,
	DIV,
	EXIT
};


void AlternateScreenOn();
void AlternateScreenOff();
void Clear();
void PrintMenu();
void AddMenu();
void SubMenu();
void MulMenu();
void DivMenu();
void Output(int get);
void MainScreen();
