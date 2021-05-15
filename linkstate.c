#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1000
#define MAX_NODE 100

void Dijkstra(int node, int src, int(*adj_graph)[100]);
void find_p(int src, int dest, int dst, FILE *fp);
int table[100][100][2];

int main(){//int argc, char *argv[]){
		
	char topofile[MAX_STR] = { '\0', };
	char mesgfile[MAX_STR] = { '\0', };
	char chgefile[MAX_STR] = { '\0', };
	char mesg[MAX_STR] = { '\0', };
	char buf[MAX_STR] = { '\0', };
	char *tmp1;
	
	int node; 	
	int src; 		// ��� ����
	int n1, n2, cost;
	int adj_graph[MAX_NODE][MAX_NODE]; // �ִ� 100�� ������ vertex�� ����
	int i, j;
	
	FILE * fwp;
	FILE * fp;
	
	// 1. ���ڼ�  
	scanf("%[^\n]", buf);
	/*if(argc != 4){
		fprintf(stderr, "usage: linkstate topologyfile messagesfile changefile\n"); 
 		exit(1); 
	}*/
	
	// 2. parsing
	tmp1 = strtok(buf," ");   //tmp1 = strtok(argv[1]," ");
	strcpy(topofile, tmp1);
	tmp1 = strtok(NULL, " ");
	strcpy(mesgfile, tmp1);
	tmp1 = strtok(NULL, " ");
	strcpy(chgefile, tmp1);

	// 3-1. topology open 
	fp = fopen(topofile, "r");
	fwp = fopen("output_ls.txt","w");
	if (fp == NULL) {
		printf("Error: open input file.\n");
		return -1;
	}
	
	fgets(buf, MAX_STR, fp);
	sscanf(buf, "%d", &node);
	for(i = 0; i < node; i++)
		for(j = 0; j < node; j++){
			adj_graph[i][j] = 999;
			table[i][j][0] = 999;
			table[i][j][1] = 999;
		}
			
			
	while(fgets(buf, MAX_STR, fp)){
		sscanf(buf, "%d %d %d", &n1, &n2, &cost);
		adj_graph[n1][n2] = cost;
		adj_graph[n2][n1] = cost;
	}
	
	for(i = 0; i < node; i++ ){
		// �ʱ�ȭ
		table[i][i][1] = 0;
		Dijkstra(node, i, adj_graph);
	}
	for(i =0; i < node; i++){
		for(j = 0; j < node; j++){
			printf("%d %d %d\n",j, table[j][i][0], table[j][i][1]);
		}
		printf("\n");
	}
	fclose(fp);

	// 3-2. message
	fp = fopen(mesgfile, "r");
	if (fp == NULL) {
		printf("Error: open input file.\n");
		return -1;
	}
		
	while(fgets(buf, MAX_STR, fp)){
		sscanf(buf, "%d %d %s", &n1, &n2, mesg);
		strcpy(mesg,buf+4);
		if(table[n1][n2][1] == 999){
			printf("from %d to %d cost infinite hops unreachable message %s\n",n1,n2,mesg);
		}
		else{
			printf("from %d to %d cost %d hops ",n1,n2,table[n1][n2][1]);
			find_p(n1,n2,n2,fp);
			printf("message %s\n",mesg);
		}
	}
	fclose(fp);
	
	
	// 3-3. changes
	
	// 4. finish
	printf("Complete. Output file written to ouput_ls.txt.\n"); 
	
}
void find_p(int src, int dest, int dst, FILE *fp) {
	if (dest == src) {
		printf("%d ", dest);
		return;
	}
	else {
		find_p(src, table[src][dest][0], dst ,fp); //�������� ������� �ƴѰ�� �����ϴ� ���� Ž��
		if(dest != dst)
			printf("%d ", dest);
		return;
	}
}
void Dijkstra(int node, int src, int(*adj_graph)[100]) {

	int min; 		// �ִ� ��ΰ� ������ vertex�� dist �� 
	int index;		// �ִ� ��ΰ� ������ vertex �� ���� ���� ���� index
	int i, j, k;
	int found[MAX_NODE]; // vertex �湮 üũ ǥ��, 0�� false, 1�� true (1�� �ִܰ�ΰ� �����ǰ� �湮�Ѵٴ� �ǹ��̴�)   
	// neighbor �迭�� �̿��ϴ� Dijkstra �˰����� �ۼ��մϴ�.
	// �ִ� ��ΰ� �������� ���� vertex�� found�� 0(false)���� �ʱ�ȭ 
	for (i = 0; i < node; i++) {
		found[i] = 0; 
	}
	
	table[src][src][0] = src; // ������� �̿������� �ڱ� �ڽ��̹Ƿ� �ڱ��ڽ��� ���� �Է��Ѵ�. 
	found[src] = 1; // ������������� ��θ� Ž���ϹǷ� �湮 üũ 1�� ǥ���Ѵ�.  
	index = src; // index�� �ʱ�ȭ�� '�����'���� ���� 

	for (i = 0; i < node - 1; i++) { //�� n-1���� �ݺ��Ѵ�.
		for (j = 0; j < node; j++) { // �ּҰŸ��� index�� ����� ���� �ּҰŸ��� ���� 
			if ((table[src][index][1] + adj_graph[index][j]) < table[src][j][1]) { // ������ �Ÿ����� index�� ������ �Ÿ��� �� ª�� ���, �����ϴ� �Ÿ��� dist�� ���� �����Ѵ�.
				table[src][j][1] = table[src][index][1] + adj_graph[index][j];
				table[src][j][0] = index;
			}
		}
		// found�� ���� �����鼭 �ּҰŸ��� ���� vertex�� ã��. 
		min = 9999;
		index = 0;
		for (k = 0; k < node; k++) {
			if ((table[src][k][1] < min) && (!found[k])) {
				min = table[src][k][1]; // �ּ� �Ÿ��� ���� ���� 
				index = k; // �ּ� �Ÿ��� index ���� ���� 
			}
		}
		found[index] = 1; // index�� �湮�Ѵٴ� ǥ��. 
	}
}
