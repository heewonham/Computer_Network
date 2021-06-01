#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1000
#define MAX_NODE 100

int topology(FILE *fwp, char* topofile, int *node);
int message(FILE *fwp, char* mesgfile);
int change(FILE* fwp, char* chgefile, char* mesgfile, int node);
void Dijkstra(int node, int src);
void write_route(int src, int dest, int dst, FILE* fwp);		// 경로 출력 
int find_parent(int src, int dest, int prev);				// parent 찾기 

int table[MAX_NODE][MAX_NODE][2];
int adj_graph[MAX_NODE][MAX_NODE];

int main(int argc, char *argv[]){
		
	int node; 		
	FILE * fwp = fopen("output_ls.txt","w");
	
	// 1. 인자수  
	if(argc != 4){
		fprintf(stderr, "usage: linkstate topologyfile messagesfile changefile\n"); 
 		exit(1); 
	}

	// 2-1. topology open 
	if(topology(fwp, argv[1], &node) == -1)
		exit(1);

	// 2-2. message
	if(message(fwp, argv[2]) == -1)
		exit(1);
	
	// 2-3. changes
	if(change(fwp, argv[3], argv[2], node) == -1)
		exit(1);
	
	// 3. finish
	printf("Complete. Output file written to ouput_ls.txt.\n"); 
	
}
void Dijkstra(int node, int src) {

	int min; 		
	int index;		
	int i, j, k;
	int found[MAX_NODE]; 		// vertex 방문 체크 표시  

	for (i = 0; i < node; i++) {
		found[i] = 0; 
	}
	
	table[src][src][0] = src; 	// 출발점의 이웃점정은 자기 자신 
	found[src] = 1; 			// 출발점에서부터 경로를 탐색하므로 방문 체크 1을 표시한다.  
	index = src; 				// index의 초기화를 '출발점'으로 지정 

	for (i = 0; i < node - 1; i++) { 
		for (j = 0; j < node; j++) { 
			if ((table[src][index][1] + adj_graph[index][j]) < table[src][j][1] 
			|| ((table[src][index][1] + adj_graph[index][j]) == table[src][j][1] && table[src][j][0] > index)) { 
				// 값이 같을 경우, parent가 작은 것으로 갱신 
				table[src][j][1] = table[src][index][1] + adj_graph[index][j];
				table[src][j][0] = index;
			}
		}

		min = 9999;
		index = 0;
		for (k = 0; k < node; k++) {
			if ((table[src][k][1] < min) && (!found[k])) {
				min = table[src][k][1]; 
				index = k; 
			}
		}
		found[index] = 1;
	}
}
void write_route(int src, int dest, int dst, FILE* fwp) {
	if (dest == src) {
		fprintf(fwp,"%d ", dest);
		return;
	}
	else {
		write_route(src, table[src][dest][0], dst ,fwp); 
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
		return find_parent(src, table[src][dest][0], dest);
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
	
	// 초기화 
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
		// 초기화
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

