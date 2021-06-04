#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1000
#define MAX_NODE 100

int main(){//int argc, char *argv[]){
		
	int i, bi, num;
	char input[MAX_STR], output[MAX_STR];
	char buf;
	int generator, dataword_size;
	int result[20];
	FILE *fwp, *fp;
	
	scanf("%s %s %d %d", input, output, &generator, &dataword_size);
	fp = fopen(input, "rb");
	
	while(fread(&buf, 1, 1, fp)){
		
		num = buf[i]-'0';
		for(i = 7; i >= 0; --i){
			result[7-i] = num >> i & 1;
		}
		
	}
	
	/* 1. ÀÎÀÚ¼ö  
	if(argc != 4){
		fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n"); 
 		exit(1); 
	}
	
	fp = fopen(input, "r");
	if (fp == NULL) {
		printf("input file open error.\n");
		return -1;
	}
	
	fwp = fopen(output, "w");
	if (fwp == NULL) {
		printf("output file open error./n");
		return -1;
	}
	
	if (dataword_size != 4 && dataword_size != 8){
		printf("dataword size must be 4 or 8./n");
		return -1;
	*/
}
void 
