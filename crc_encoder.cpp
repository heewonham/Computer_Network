#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#define MAX 1000

using namespace std;
string binary;
vector<char> result;

void make_binary(int num);      // char to binary
int make_dec(string num);       // string binary to dec
void crc_encode(string dataword, string generator);
void write_output(FILE* fwp);

int main(int argc, char *argv[]){

        int i, j, dw_size;
        string generator, dataword;
        char buf[MAX] = {'\0',}, input[MAX] = { '\0', }, output[MAX] = { '\0', };
        FILE* fwp, * fp;

		cin >> input >> output >> generator >> dw_size;

        /* 1. 인자수
        if(argc != 5){
                fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n");
                exit(1);
		}*/
		ifstream fin(input, ios::binary);
		fin.seekg(0, ios::end);
		int size = fin.tellg();
		fin.seekg(0,ios::beg);
		
		fin.read(buf, size);
		cout << size << endl;
		fin.close();
        /* 2. input file open
        fp = fopen(input, "rb");
        if (fp == NULL) {
                printf("input file open error.\n");
                return -1;
        }

        // 3. output file open
        fwp = fopen(output, "wb");
        if (fwp == NULL) {
                printf("output file open error.\n");
                return -1;
        }

        // 4. dataword size check
        if (dw_size != 4 && dw_size != 8) {
                printf("dataword size must be 4 or 8.\n");
                return -1;
        }

        while (fread(buf, sizeof(char), MAX, fp)) {
                for(i=0; i< (int)strlen(buf); i++){
                        if((int)buf[i] != 13){
                                // 1. 이진수 변환
                                make_binary((int)buf[i]);
                                // 2. crc_decode
                                for (j = 0; j < 8; j++) {
                                        dataword += binary.at(j);
                                        if ((int)dataword.size() == dw_size) {
                                                crc_encode(dataword, generator);
                                                dataword = "";
                                        }
                                }
                        }
                }
                memset(buf, '\0', MAX);
        }

        write_output(fwp);
        fclose(fp);
        fclose(fwp);*/
}
int make_dec(string num) {
        int i,dec = 0;
        for (i = 0; i < (int)num.size(); i++)
                dec = (dec << 1) + num.at(i) - '0';
        return dec;
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
void crc_encode(string dataword, string generator) {
        int i, j, num;
        string tmp, operand = "", data = dataword;

        for (i = 0; i < (int)generator.size()-1; i++)
                data += "0";
        operand = data.substr(0, (int)generator.size());

        for (i = (int)generator.size(); i <= (int)data.size(); i++) {
                if (operand.at(0) == '0')
                        operand.erase(0, 1);
                else {
                        tmp = "";
                        for (j = 1; j < (int)generator.size(); j++) {
                                num = (operand.at(j) - '0') ^ (generator.at(j) - '0');
                                tmp += (num + '0');
                        }
                        operand = tmp;
                }
                if (i < (int)data.size())
                        operand += data.at(i);
        }

        for (i = 0; i < (int)dataword.size(); i++)
                result.push_back(dataword.at(i));
        for (i = 0; i < (int)operand.size(); i++)
                result.push_back(operand.at(i));
}
void write_output(FILE* fwp) {

        int i, padsize, cnt;
        char buf;
        string codeword = "";

        // 1. cal padding size
        padsize = 8 - (result.size() % 8);
        buf = (char)padsize;
        fwrite(&buf, sizeof(char), 1, fwp);

        // 2. including padding
        for (i = 0; i < padsize; i++) {
                codeword += "0";
        }

        cnt = padsize;
        // 3. codeword
        for (i = 0; i < (int)result.size(); i++) {
                codeword += result[i];
                cnt++;
                if (cnt == 8) {
                        buf = make_dec(codeword);
						fwrite(&buf, sizeof(char), 1, fwp);
                        codeword = "";
                        cnt = 0;
                }
        }
}

