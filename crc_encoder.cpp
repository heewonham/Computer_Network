#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
# define MAX 1000

using namespace std;
string binary;
vector<int> result;

void make_binary(char buf);
void crc_encode(string dataword, string generator, int datasize);

int main() {//int argc, char *argv[]){

	int i, j, padsize;
	int datasize, len = 0;
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
		len += strlen(buf);
		for(i=0; i< len; i++){
			// 1. 이진수 변환
			make_binary(buf[i]);
			// 2. crc_decode
			for (j = 0; j < 8; j++) {
				dataword += binary.at(j);
				if (dataword.size() == datasize) {
					crc_encode(dataword, generator, datasize);
					dataword = "";
				}
			}
		}
	}

	/*padsize = 8 - (result.size() % 8);
	while (1) {

	}*/
}
void make_binary(char buf) {
	int i, num = (int)buf;
	char tmp;

	for (i = 7; i >= 0; --i) {
		tmp = (num >> i & 1)+'0';
		binary += tmp;
	}
}
void crc_encode(string dataword, string generator, int datasize) {
	int i, j, num;
	int gen = stoi(generator);
	string reminder, operand, data = dataword;
	string tmp;

	for (i = 0; i < generator.size()-1; i++)
		data += "0";
	operand = data.substr(0, datasize);

	//for (i = datasize; i < data.size(); i++) {
	i = datasize;
	while(i <= data.size()){
		if (operand.at(0) == '0') {
			operand.erase(0, 1);
			operand += data.at(i++);
		}
		else {
			tmp = "";
			for (j = 0; j < operand.size(); j++) {
				num = operand.at(j) - '0' ^ generator.at(j) - '0';
				tmp += num + '0';
			}
			operand = tmp;
			cout << operand << endl;
		}
	} 
}
