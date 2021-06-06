#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1000
#define MAX_NODE 100

int main(){//int argc, char *argv[]){
		
	int i, j;
	int count, tmp, bitsize;
	char input[MAX_STR], output[MAX_STR], generator[8], buf[MAX_STR];
	int dataword;
	int result[20];
	FILE *fwp, *fp;
	
	scanf("%s %s %s %d", input, output, generator, &dataword);
	
	/* 1. ÀÎÀÚ¼ö  
	if(argc != 4){
		fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n"); 
 		exit(1); 
	}*/
	
	fp = fopen(input, "rb");
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
	
	// 1. main
	while(fread(&buf, sizeof(char), MAX_STR, fp)){
		
		count = (strlen(buf) * 8) / datasize;
		tmp = (datasize + strlen(generator) -1) * count;
		bitsize = tmp + (8 - (tmp % 8));
		fwrite(buf,sizeof(int),8, fwp);
		for(i = 0; i < strlen(buf); i++){
					//num = buf-'0';
		//for(i = 7; i >= 0; --i){
		//	result[7-i] = num >> i & 1;
		//}
		}

		//for(i = 0; i < 8; i++){
		//	printf("%d ",result[i]);
		//}
	}
} 
// 1. 
