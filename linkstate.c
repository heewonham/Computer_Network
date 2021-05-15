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
	int src; 		// 출발 정점
	int n1, n2, cost;
	int adj_graph[MAX_NODE][MAX_NODE]; // 최대 100개 이하의 vertex만 들어옴
	int i, j;
	
	FILE * fwp;
	FILE * fp;
	
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
		// 초기화
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
		find_p(src, table[src][dest][0], dst ,fp); //도착점이 출발점이 아닌경우 경유하는 점을 탐색
		if(dest != dst)
			printf("%d ", dest);
		return;
	}
}
void Dijkstra(int node, int src, int(*adj_graph)[100]) {

	int min; 		// 최단 경로가 결정된 vertex의 dist 값 
	int index;		// 최단 경로가 결정된 vertex 중 가장 작은 값의 index
	int i, j, k;
	int found[MAX_NODE]; // vertex 방문 체크 표시, 0은 false, 1은 true (1은 최단경로가 결정되고 방문한다는 의미이다)   
	// neighbor 배열을 이용하는 Dijkstra 알고리즘을 작성합니다.
	// 최단 경로가 결정되지 않은 vertex의 found를 0(false)으로 초기화 
	for (i = 0; i < node; i++) {
		found[i] = 0; 
	}
	
	table[src][src][0] = src; // 출발점의 이웃점정은 자기 자신이므로 자기자신의 값을 입력한다. 
	found[src] = 1; // 출발점에서부터 경로를 탐색하므로 방문 체크 1을 표시한다.  
	index = src; // index의 초기화를 '출발점'으로 지정 

	for (i = 0; i < node - 1; i++) { //총 n-1번을 반복한다.
		for (j = 0; j < node; j++) { // 최소거리의 index와 연결된 점의 최소거리를 갱신 
			if ((table[src][index][1] + adj_graph[index][j]) < table[src][j][1]) { // 기존의 거리보다 index를 경유한 거리가 더 짧을 경우, 경유하는 거리로 dist의 값을 갱신한다.
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
