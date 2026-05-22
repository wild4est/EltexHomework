#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	FILE *fptr;
	char* string = "String from file";
	int string_size = strlen(string);

	fptr = fopen("output.txt", "w+");
	fprintf(fptr,"%s", string);

	printf("Итоговая строка: ");
	for (int i=1; i<(string_size+1); i++) {
		fseek(fptr, -i, SEEK_END);
		char ch;
		fread(&ch, 1, 1, fptr);
		printf("%c", ch);
	}
	printf("\n");

	fclose(fptr);
}
