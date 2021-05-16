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
	char buf[MAX_STR] = { '\0', }; // 삭 제 
	char *tmp1;
	
	int node; 		
	FILE * fwp = fopen("output_ls.txt","w");
	
	// 1. 인자수  
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

	int min; 		// 최단 경로가 결정된 vertex의 dist 값 
	int index;		// 최단 경로가 결정된 vertex 중 가장 작은 값의 index
	int i, j, k;
	int found[MAX_NODE]; // vertex 방문 체크 표시, 0은 false, 1은 true (1은 최단경로가 결정되고 방문한다는 의미이다)   

	for (i = 0; i < node; i++) {
		found[i] = 0; 
	}
	
	table[src][src][0] = src; // 출발점의 이웃점정은 자기 자신이므로 자기자신의 값을 입력한다. 
	found[src] = 1; // 출발점에서부터 경로를 탐색하므로 방문 체크 1을 표시한다.  
	index = src; // index의 초기화를 '출발점'으로 지정 

	for (i = 0; i < node - 1; i++) { //총 n-1번을 반복한다.
		for (j = 0; j < node; j++) { // 최소거리의 index와 연결된 점의 최소거리를 갱신 
			if ((table[src][index][1] + adj_graph[index][j]) < table[src][j][1] 
			|| ((table[src][index][1] + adj_graph[index][j]) == table[src][j][1] && table[src][j][0] > index)) { // 기존의 거리보다 index를 경유한 거리가 더 짧을 경우, 경유하는 거리로 dist의 값을 갱신한다.
				table[src][j][1] = table[src][index][1] + adj_graph[index][j];
				table[src][j][0] = index;
			}
		}
		// found가 되지 않으면서 최소거리를 가진 vertex를 찾음. 
		min = 9999;
		index = 0;
		for (k = 0; k < node; k++) {
			if ((table[src][k][1] < min) && (!found[k])) {
				min = table[src][k][1]; // 최소 거리의 값을 저장 
				index = k; // 최소 거리의 index 값을 저장 
			}
		}
		found[index] = 1; // index를 방문한다는 표시. 
	}
}
int write_route(int src, int dest, int dst, FILE* fwp) {
	if (dest == src) {
		fprintf(fwp,"%d ", dest);
		return;
	}
	else {
		write_route(src, table[src][dest][0], dst ,fwp); //도착점이 출발점이 아닌경우 경유하는 점을 탐색
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
		return find_parent(src, table[src][dest][0], dest); //도착점이 출발점이 아닌경우 경유하는 점을 탐색
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
