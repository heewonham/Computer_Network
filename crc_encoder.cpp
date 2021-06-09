#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
# define MAX 1000

using namespace std;
string binary;
vector<char> result;

void make_binary(int num);
void crc_encode(string dataword, string generator, int datasize);
void write_output(FILE* fwp);

int main() {//int argc, char *argv[]){

	int i, j, datasize;
	string generator, dataword;
	char buf[MAX] = {'\0',} , input[MAX] = { '\0', }, output[MAX] = { '\0', };
	FILE* fwp, * fp;

	cin >> input >> output >> generator >> datasize;

	/* 1. 인자수
	if(argc != 4){
		fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n");
		exit(1);
	}*/

	fp = fopen(input, "rb");
	if (fp == NULL) {
		printf("input file open error.\n");
		return -1;
	}

	fwp = fopen(output, "wb");
	if (fwp == NULL) {
		printf("output file open error./n");
		return -1;
	}

	if (datasize != 4 && datasize != 8) {
		printf("dataword size must be 4 or 8./n");
		return -1;
	}

	while (fread(buf, sizeof(char), MAX, fp)) {
		for(i=0; i< (int)strlen(buf); i++){
			// 1. 이진수 변환
			make_binary((int)buf[i]);
			// 2. crc_decode
			for (j = 0; j < 8; j++) {
				dataword += binary.at(j);
				if ((int)dataword.size() == datasize) {
					crc_encode(dataword, generator, datasize);
					dataword = "";
				}
			}
		}
	}

	write_output(fwp);
	fclose(fp);
	fclose(fwp);
}
void make_binary(int num) {

	int i;
	char tmp;
	binary = "";

	for (i = 7; i >= 0; --i) {
		tmp = (num >> i & 1)+'0';
		binary += tmp;
	}
}
void crc_encode(string dataword, string generator, int datasize) {
	int i, j, num;
	string tmp, operand, data = dataword;

	for (i = 0; i < (int)generator.size()-1; i++)
		data += "0";
	operand = data.substr(0, datasize);

	for (i = datasize; i <= (int)data.size(); i++) {
		if (operand.at(0) == '0')
			operand.erase(0, 1);
		else {
			tmp = "";
			for (j = 1; j < (int)operand.size(); j++) {
				num = (operand.at(j) - '0') ^ (generator.at(j) - '0');
				tmp += num + '0';
			}
			operand = tmp;
		}
		if (i < (int)data.size())
			operand += data.at(i);
	}

	for (i = 0; i < (int)dataword.size(); i++)
		result.push_back(dataword[i]);
	for (i = 0; i < (int)operand.size(); i++)
		result.push_back(operand[i]);
}
void write_output(FILE* fwp) {
	
	int i, padsize;
	char buf;
	string codeword = "";

	// 1. 패딩사이즈 
	padsize = 8 - (result.size() % 8);
	make_binary(padsize);
	buf = (char)stoi(binary);
	fwrite(&buf, sizeof(char), 1, fwp);

	// 2. 패딩포함
	for (i = 0; i < padsize; i++) {
		codeword += "0";
	}

	// 3. codeword 
	for (i = 0; i < (int)result.size(); i++) {
		codeword += result[i];
		if ((i + padsize+1) % 8 == 0) {
			buf = (char)stoi(codeword);
			fwrite(&buf, sizeof(char), 1, fwp);
			codeword = "";
		}
	}
}
