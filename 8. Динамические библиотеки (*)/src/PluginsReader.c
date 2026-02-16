#include "PluginsReader.h"


void AlternateScreenOn(){
	printf("\033[?1049h\033[H");
}
  
void AlternateScreenOff(){
	printf("\033[?1049l");
}

void Clear(){
	printf("\033[H\033[J");
}


int GetFirstNChar(char* output, char* input, int n){
	if (strlen(input) < n ) {
		return 1;
	}
	for (int i=3; i<n+3; i++) {
		output[i-3] = input[i];
	}
	output[n] = '\0';
	return 0;
}


int ReadPlugins(struct OperationLib** operation_libs, int* n){
	struct dirent **namelist;
	int dir_n;

	dir_n = scandir("plugins", &namelist, NULL, alphasort);
	if (dir_n == -1) {
		return 1;
	}
	
	struct OperationLib* operation_libs_local = malloc(sizeof(struct OperationLib)*(dir_n-2));
	*n = 0;
	for (int i=0; i<dir_n; i++) {
		
		char dir_plug[20] = "plugins/";
		char operation[4];

		strcat(dir_plug, namelist[i]->d_name);

		operation_libs_local[*n].handle = dlopen(dir_plug, RTLD_LAZY);
		if (!operation_libs_local[*n].handle) {
			continue;
		}
		
		GetFirstNChar(operation, namelist[i]->d_name, 3);
		operation_libs_local[*n].func_ptr = dlsym(operation_libs_local[*n].handle, operation);
		strcpy(operation_libs_local[*n].oper_name, operation);
		
		(*n)++;
	}

	for (int i=0; i<dir_n; i++) {
		free(namelist[i]);
	}	
	free(namelist);

	*operation_libs = operation_libs_local;
	return 0;
}

void PrintMenu(struct OperationLib* operation_libs, int n){
	printf("---------------------------------------------\n");
	printf("Введите один из пунктов меню:\n");
	printf("%d. Выход\n", EXIT);
	for(int i=0; i<n; i++){
		printf("%d. %s\n", (i+EXIT+1), operation_libs[i].oper_name);
	}

}

int UseOperation(int (*func_ptr)(int*, int, int)){
	Clear();
        printf("Введите первый операнд:\n");
        int a;
	scanf("%d", &a);
 
        Clear();
        printf("Введите второй операнд:\n");
        int b;
        scanf("%d", &b);
 
        Clear();
        int answer;
        func_ptr(&answer, a, b);
        printf("Ваш ответ:\n %d\n", answer);

	return 0;
}

int Output(){
	AlternateScreenOn();

	struct OperationLib* operation_libs;
	int n;
	ReadPlugins(&operation_libs, &n);
	int get;
	do {
		PrintMenu(operation_libs, n);
		scanf("%d", &get);
		if (get == EXIT) {
			break;
		}
		if (get>(n+1)) {
			Clear();
			continue;
		}
		UseOperation(operation_libs[get-2].func_ptr);
	} while (1);
	
	for(int i=0; i<n; i++){
		dlclose(operation_libs[i].handle);
	}
	free(operation_libs);

	AlternateScreenOff();

	return 0;
}
