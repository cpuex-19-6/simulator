#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "functions.h"

uint32_t downto(uint32_t u, int n, int m){
	uint32_t ans = (u << (31-n)) >> (31- n + m);
	return ans;
}

//テキストで表されたデータをバイナリに変換　ただし、8ビットずつ
//命令メモリの方で使うものなので、エンディアンは調整していない（遅くなるから）
int c2b_8(uint8_t *dest, char *source, size_t size){
	int ans = 0;
	int count = 0;
	uint8_t tmp = 0;

	for(int i = 0; i < size; i++){
		switch(source[i]){
			case '0':
				count++;
				break;
			case '1':
				tmp = tmp | (1 << (7 - count));
				count++;
				break;
			default:
				break;
		}
		if(count == 8){
			ans += count;
			*dest = tmp;
			count = 0;
			dest++;
			tmp = 0;
		}
	}	

	return ans;
}

int c2b_32(uint32_t *dest, char *source, size_t size){
	int ans = 0;
	int count = 0;
	uint32_t tmp = 0;

	for(int i = 0; i < size; i++){
		switch(source[i]){
			case '0':
				count++;
				break;
			case '1':
				tmp = tmp | (1 << (31 - count));
				count++;
				break;
			default:
				break;
		}
		if(count == 32){
			ans += count;
			*dest = tmp;
			count = 0;
			dest++;
			tmp = 0;
		}
	}	

	return ans;
}

void b2c_32(char *dest, int32_t source){
	int32_t check = 1 << 31;
	for(int i = 0; i < 32; i++){
		if(source & check)
			dest[i] = '1';
		else dest[i] = '0'; 
		source = source << 1;
	}
}

void b2c_8(char *dest, int8_t source){
	int8_t check = 1 << 7;
	for(int i = 0; i < 8; i++){
		if(source & check)
			dest[i] = '1';
		else dest[i] = '0'; 
		source = source << 1;
	}
}

void print_binary(uint32_t instr){
	uint32_t check = 1 << 31;
	char s[33];

	for(int i = 0; i < 32; i++){
		if(check & instr){
			s[i] = '1';
		}
		else {
			s[i] = '0';
		}
		instr = instr << 1;
	}
	s[32] = '\0';

	printf("%s", s);
}

//copy b to a with endian wrapping
void endian_wrapper(void *a, void *b, size_t size){
	for(int i = 0; i < size; i++)
		memcpy(a + size -1 - i, b + i, 1);
}
