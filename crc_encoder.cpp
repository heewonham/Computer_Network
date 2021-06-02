#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1000
#define MAX_NODE 100

int main(){//int argc, char *argv[]){
		
	char input[MAX_STR], output[MAX_STR];
	int generator, dataword_size;
	FILE *fwp, *fp;
	
	scanf("%s %s %d %d", input, output, &generator, &dataword_size);
	
	
	printf("%s %s %d %d", input, output, generator, dataword_size);
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
