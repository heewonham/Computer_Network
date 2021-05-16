#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1000
#define MAX_NODE 100

int topology(FILE *fwp, char* topofile, int *node);
int message(FILE *fwp, char* mesgfile);
int change(FILE* fwp, char* chgefile, char* mesgfile, int node);
void Dijkstra(int node, int src);
int write_route(int src, int dest, int dst, FILE* fwp);
int find_parent(int src, int dest, int prev);

int table[MAX_NODE][MAX_NODE][2];
int adj_graph[MAX_NODE][MAX_NODE];

int main(){//int argc, char *argv[]){
		
	char topofile[MAX_STR] = { '\0', };
	char mesgfile[MAX_STR] = { '\0', };
	char chgefile[MAX_STR] = { '\0', };
	char buf[MAX_STR] = { '\0', }; // �� �� 
	char *tmp1;
	
	int node; 		
	FILE * fwp = fopen("output_ls.txt","w");
	
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
	if(topology(fwp, topofile, &node) == -1)
		exit(1);

	// 3-2. message
	if(message(fwp, mesgfile) == -1)
		exit(1);
	
	// 3-3. changes
	if(change(fwp, chgefile, mesgfile, node) == -1)
		exit(1);
	
	// 4. finish
	printf("Complete. Output file written to ouput_ls.txt."); 
	
}
void Dijkstra(int node, int src) {

	int min; 		// �ִ� ��ΰ� ������ vertex�� dist �� 
	int index;		// �ִ� ��ΰ� ������ vertex �� ���� ���� ���� index
	int i, j, k;
	int found[MAX_NODE]; // vertex �湮 üũ ǥ��, 0�� false, 1�� true (1�� �ִܰ�ΰ� �����ǰ� �湮�Ѵٴ� �ǹ��̴�)   

	for (i = 0; i < node; i++) {
		found[i] = 0; 
	}
	
	table[src][src][0] = src; // ������� �̿������� �ڱ� �ڽ��̹Ƿ� �ڱ��ڽ��� ���� �Է��Ѵ�. 
	found[src] = 1; // ������������� ��θ� Ž���ϹǷ� �湮 üũ 1�� ǥ���Ѵ�.  
	index = src; // index�� �ʱ�ȭ�� '�����'���� ���� 

	for (i = 0; i < node - 1; i++) { //�� n-1���� �ݺ��Ѵ�.
		for (j = 0; j < node; j++) { // �ּҰŸ��� index�� ����� ���� �ּҰŸ��� ���� 
			if ((table[src][index][1] + adj_graph[index][j]) < table[src][j][1] 
			|| ((table[src][index][1] + adj_graph[index][j]) == table[src][j][1] && table[src][j][0] > index)) { // ������ �Ÿ����� index�� ������ �Ÿ��� �� ª�� ���, �����ϴ� �Ÿ��� dist�� ���� �����Ѵ�.
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
int write_route(int src, int dest, int dst, FILE* fwp) {
	if (dest == src) {
		fprintf(fwp,"%d ", dest);
		return;
	}
	else {
		write_route(src, table[src][dest][0], dst ,fwp); //�������� ������� �ƴѰ�� �����ϴ� ���� Ž��
		if(dest != dst)
			fprintf(fwp,"%d ", dest);
		return;
	}				 
}
int find_parent(int src, int dest, int prev) {
	if (dest == src) {
		return prev;
	}
	else {
		return find_parent(src, table[src][dest][0], dest); //�������� ������� �ƴѰ�� �����ϴ� ���� Ž��
	}				 
}
int topology(FILE *fwp, char* topofile, int *node) {
	
	char buf[MAX_STR] = { '\0', };
	FILE * fp = fopen(topofile, "r");
	int n1, n2, cost;
	int i, j;

	if (fp == NULL) {
		printf("Error: open input file.\n");
		return -1;
	}
	
	fgets(buf, MAX_STR, fp);
	sscanf(buf, "%d", &(*node));
	
	// �ʱ�ȭ 
	for(i = 0; i < *node; i++)
		for(j = 0; j < *node; j++){
			adj_graph[i][j] = 999;
			table[i][j][0] = 999;
			table[i][j][1] = 999;
		}
			
			
	while(fgets(buf, MAX_STR, fp)){
		sscanf(buf, "%d %d %d", &n1, &n2, &cost);
		if(cost == -999) cost = 999;
		adj_graph[n1][n2] = cost;
		adj_graph[n2][n1] = cost;
	}
	
	for(i = 0; i < *node; i++ ){
		// �ʱ�ȭ
		table[i][i][1] = 0;
		Dijkstra(*node, i);
	}
	for(i =0; i < *node; i++){
		for(j = 0; j < *node; j++){
			if(table[i][j][1] != 999)
				fprintf(fwp,"%d %d %d\n",j, find_parent(i,j,i),table[i][j][1]);
		}
		fprintf(fwp,"\n");
	}
	fclose(fp);
	return 0;
}

int message(FILE *fwp, char* mesgfile) {
	
	char buf[MAX_STR] = { '\0', };
	char mesg[MAX_STR] = { '\0', };
	FILE * fp = fopen(mesgfile, "r");
	int n1, n2;
	
	if (fp == NULL) {
		printf("Error: open input file.\n");
		return -1;
	}
		
	while(fgets(buf, MAX_STR, fp)){
		sscanf(buf, "%d %d %s", &n1, &n2, mesg);
		strcpy(mesg,buf+4);
		if(table[n1][n2][1] == 999){
			fprintf(fwp,"from %d to %d cost infinite hops unreachable message %s\n",n1,n2,mesg);
		}
		else{
			fprintf(fwp,"from %d to %d cost %d hops ",n1,n2,table[n1][n2][1]);
			write_route(n1, n2, n2, fwp);
			fprintf(fwp,"message %s",mesg);
		}
	}
	fprintf(fwp,"\n");
	fclose(fp);
	return 0;
}

int change(FILE* fwp, char* chgefile, char* mesgfile, int node) {
	
	char buf[MAX_STR] = { '\0', };
	FILE * fp = fopen(chgefile, "r");
	int n1, n2, cost;
	int i, j;

	if (fp == NULL) {
		printf("Error: open input file.\n");
		return -1;
	}
		
	while(fgets(buf, MAX_STR, fp)){
		sscanf(buf, "%d %d %d", &n1, &n2, &cost);
		if(cost == -999) cost = 999;
		if(adj_graph[n1][n2] < cost){
			for(i = 0; i < node; i++){
				for(j = 0; j < node; j++){
					table[i][j][0] = 999;
					table[i][j][1] = 999;
				}
			}
		}

		adj_graph[n1][n2] = cost;
		adj_graph[n2][n1] = cost;
		
		for(i = 0; i < node; i++ ){
			table[i][i][1] = 0;
			Dijkstra(node, i);
		}
		for(i =0; i < node; i++){
			for(j = 0; j < node; j++){
				if(table[i][j][1] != 999)
					fprintf(fwp,"%d %d %d\n",j, find_parent(i,j,i),table[i][j][1]);
			}
			fprintf(fwp,"\n");
		}
		
		message(fwp,mesgfile);
	}
	
	fclose(fp);
	return 0;
}
