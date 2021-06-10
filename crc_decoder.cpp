#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
# define MAX 1000

using namespace std;
string binary;
vector<char> outdata;
pair<int, int> result(0,0);		// codeword num, error num

void make_binary(int num);
void crc_decode(string generator, string codeword, int dw_size, int cw_size);
void write_output(FILE* fwp,ofstream fout);

int main() {//int argc, char *argv[]){

	int i, j, cnt;
	int dw_size;										// dataword size
	int pd_size, cw_size;						// padding size , codeword size
	char ch;

	string generator, codeword = "";
	char buf[MAX] = { '\0', }, input[MAX] = { '\0', }, output[MAX] = { '\0', }, result_file[MAX] = { '\0', };
	FILE* fwp, * fp, *fwp_r;
	ofstream fout;

	cin >> input >> output >> result_file >>generator >> dw_size;

	/* 1. 인자수
	if(argc != 6){
		fprintf(stderr, "usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");
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
	fout.open("copy.jpg", ios::binary);

	fwp_r = fopen(result_file, "w");
	if (fwp_r == NULL) {
		printf("result file open error./n");
		return -1;
	}

	if (dw_size != 4 && dw_size != 8) {
		printf("dataword size must be 4 or 8./n");
		return -1;
	}

	cw_size = dw_size + (int)generator.size() - 1;	// codeword size

	// 1. find padding size 
	fread(buf, sizeof(char), 2, fp);
	pd_size = (int)buf[0];

	// 2. padding 제거
	make_binary((int)buf[1]);
	cnt = 0;
	for (i = pd_size; i < (int)binary.size(); i++) {
		codeword += binary.at(i);
		cnt++;
		if (cnt % cw_size == 0) {
			crc_decode(generator, codeword, dw_size, cw_size);
			codeword = "";
			cnt = 0;
		}	
	}
	memset(buf, '\n', MAX);
	/*while (fread(buf, sizeof(char), MAX, fp)) {
		cout << strlen(buf) << endl;
		for (i = 0; i < (int)strlen(buf); i++) {
			if()
			cout << i << ": "<< buf[i] << endl;
				// 1. 이진수 변환
				make_binary((int)buf[i]);
			// 2. crc_decode
			for (j = 0; j < 8; j++) {
				codeword += binary.at(j);
				cnt++;
				if (cnt == cw_size) {
					crc_decode(generator, codeword, dw_size, cw_size);
					codeword = "";
					cnt = 0;
				}
			}
		}
		memset(buf, '\0', MAX);
	}*/
	while ((ch = fgetc(fp)) != EOF) {
		cout << strlen(buf) << endl;
		// 1. 이진수 변환
		make_binary((int)ch);
		// 2. crc_decode
		for (j = 0; j < 8; j++) {
			codeword += binary.at(j);
			cnt++;
			if (cnt == cw_size) {
				crc_decode(generator, codeword, dw_size, cw_size);
				codeword = "";
				cnt = 0;
			}
		}
	}

	// write output file
	write_output(fwp, std::fout);

	// wrtie result file
	fprintf(fwp_r, "%d %d", result.first, result.second);

	fclose(fp);
	fclose(fwp_r);
	fclose(fwp);
}
int make_dec(string num) {
	int i, dec = 0;
	for (i = 0; i < (int)num.size(); i++)
		dec = (dec << 1) + num.at(i) - '0';
	return dec;
}
void make_binary(int num) {

	int i;
	char tmp;
	binary = "";

	for (i = 7; i >= 0; --i) {
		tmp = (num >> i & 1) + '0';
		binary += tmp;
	}
}
void crc_decode(string generator, string codeword, int dw_size, int cw_size) {
	int i, j, num;
	string tmp, operand = "";

	for (i = 0; i < (int)generator.size(); i++)
		operand += codeword.at(i);

	for (; i <= cw_size; i++) {
		if (operand.at(0) == '0')
			operand.erase(0, 1);
		else {
			tmp = "";
			for (j = 1; j < (int)generator.size(); j++) {
				num = (operand.at(j) - '0') ^ (generator.at(j) - '0');
				tmp += num + '0';
			}
			operand = tmp;
		}
		if (i < (int)codeword.size())
			operand += codeword.at(i);
	}

	result.first ++;
	// error check
	for (i = 0; i < (int)operand.size(); i++) {
		if (operand.at(i) != '0')
			result.second++;
	}

	// write out 
	for (i = 0; i < dw_size; i++) {
		outdata.push_back(codeword.at(i));
	}
}
void write_output(FILE* fwp,ofstream fout) {
	int i;
	char buf;
	int num = 0;
	char buff[11000] = {'\0',};
	string dataword = "";

	for (i = 0; i < (int)outdata.size(); i++) {
		dataword += outdata[i];
		if ((int)dataword.size() == 8) {
			cout << dataword << endl;
			buf = (char)make_dec(dataword);
			cout << buf << endl;
			buff[num++] = buf;
			fwrite(&buf, sizeof(char), 1, fwp);
			dataword = "";
		}
	}
	fout.write(buff,num);
}
