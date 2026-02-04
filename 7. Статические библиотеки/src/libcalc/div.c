#include "libcalc.h"

int div(int* ans, int a, int b){
	if(b==0){
		return 1;
	}
	*ans = a/b;
	return 0;
}
