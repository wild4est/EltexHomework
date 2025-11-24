#include "BinaryFunc.h"

int powOfTwo(int num){
	int pow=0;
	int sign = 0;
	if(num<0){
		return BUFFER_SIZE;
	}
	
	int num_saved = num;
	while(num_saved!=sign){
		num_saved = num_saved >> 1;
		pow++;
	}
	return pow;
}

void reverseMasChar(char* mas, char* newmas, int n){
	for(int i=0; i<n; i++){
		newmas[i] = mas[n-1-i];
	}
}

void toBinary(int num, char* bin_num){
	int num_saved = num;
	int num_is_neg = num<0;
	int sign = 0;
	if(num_is_neg){
		sign = 0xFFFFFFFF;
	}
	int size_of_binnum = powOfTwo(num_saved);

	char rev_bin_num[size_of_binnum];
	
	int i = 0;
	while(num_saved!=sign){
		int remainder = num_saved&1;

		rev_bin_num[i] = remainder + '0';
		i++;

		num_saved = num_saved >> 1;
	}
	
	if(num_is_neg){
		while(i!=size_of_binnum){
			rev_bin_num[i] = '1';
			i++;
		}
	}

	reverseMasChar(rev_bin_num, bin_num, size_of_binnum);
	
	bin_num[size_of_binnum] = '\0';
}


int countBits(int num){
	int count = 0;

	while(num!=0){
		int remainder = num&1;
		count += remainder;
		num = num >> 1;
	}

	return count;	
}

int replaceThirdByte(int num_original, unsigned char num_replaced){
	int num_original_saved = num_original;
	int num_replaced_saved = num_replaced;

	num_original_saved = num_original_saved&0xFF00FFFF;
	num_replaced_saved = num_replaced_saved << 16;

	return num_original_saved|num_replaced_saved;	
}

