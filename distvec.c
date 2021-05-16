#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1000
#define MAX_NODE 100

int distance(int node);
int topology(FILE *fwp, char* topofile, int *node);
int message(FILE *fwp, char* mesgfile);
int change(FILE* fwp, char* chgefile, char* mesgfile, int node);

int table[MAX_NODE][MAX_NODE][2];
int adj_graph[MAX_NODE][MAX_NODE];

int main(){//int argc, char *argv[]){
		
	char topofile[MAX_STR] = { '\0', };
	char mesgfile[MAX_STR] = { '\0', };
	char chgefile[MAX_STR] = { '\0', };
	char buf[MAX_STR] = { '\0', }; // 삭 제 
	char *tmp1;
	
	int node; 	
	FILE * fwp = fopen("output_dv.txt","w");
	
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

	//3-2. message
	if(message(fwp, mesgfile) == -1)
		exit(1);
	
	// 3-3. changes
	if(change(fwp, chgefile, mesgfile, node) == -1)
		exit(1);
	
	// 4. finish
	printf("Complete. Output file written to ouput_ls.txt."); 
	
}

int topology(FILE *fwp, char* topofile, int *node) {
	
	char buf[MAX_STR] = { '\0', };
	FILE * fp = fopen(topofile, "r");
	int n1, n2, cost;
	int i, j;
	int count = 99;

	if (fp == NULL) {
		printf("Error: open input file.\n");
		return -1;
	}
	
	fgets(buf, MAX_STR, fp);
	sscanf(buf, "%d", &(*node));	
	
	// 초기화 
	for(i = 0; i < (*node); i++){
		for(j = 0; j < (*node); j++){
			if(i == j){
				adj_graph[i][j] = 0;
				table[i][j][1] = 0;
				table[i][j][0] = i;
			}
			else{
				adj_graph[i][j] = 999;
				table[i][j][1] = 999;
				table[i][j][0] = 999;
			}
		}
	}
			
	while(fgets(buf, MAX_STR, fp)){
		sscanf(buf, "%d %d %d", &n1, &n2, &cost);
		if(cost == -999) cost = 999;
		adj_graph[n1][n2] = cost;
		adj_graph[n2][n1] = cost;
	}
	
	while(count != 0){
		count = distance(*node);
	}

	for(i =0; i < *node; i++){
		for(j = 0; j < *node; j++){
			if(table[i][j][1] != 999)
				fprintf(fwp,"%d %d %d\n", j, table[i][j][0], table[i][j][1]);
		}
		fprintf(fwp,"\n");
	}
	fclose(fp);
	return 0;
}

int distance(int node) {

	int i, j, k;
	int count = 0;

	for(i = 0; i < node; i++){
		for(j = 0; j < node; j++){
			for(k = 0; k < node; k++){
				if((table[i][j][1] > (adj_graph[i][k] + table[k][j][1]))
				||(i != k && table[i][j][1] == (adj_graph[i][k] + table[k][j][1]) && table[i][j][0] > k)){
					table[i][j][1] = adj_graph[i][k] + table[k][j][1]; 
					table[i][j][0] = k;
					count++;
				}
			}
		}
	}
	return count;
}

int message(FILE *fwp, char* mesgfile) {
	
	char buf[MAX_STR] = { '\0', };
	char mesg[MAX_STR] = { '\0', };
	FILE * fp = fopen(mesgfile, "r");
	int n1, n2, src;
	
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
			src = n1;
			while(src != n2){
				fprintf(fwp,"%d ",src);
				src = table[src][n2][0];
			}
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
	int count;

	if (fp == NULL) {
		printf("Error: open input file.\n");
		return -1;
	}
	
	while(fgets(buf, MAX_STR, fp)){
		count = 99;
		sscanf(buf, "%d %d %d", &n1, &n2, &cost);
		
		if(cost == -999) cost = 999;		
		if(adj_graph[n1][n2] < cost){
			for(i = 0; i < node; i++){
				for(j = 0; j < node; j++){
					if(i == j){
						table[i][j][1] = 0;
						table[i][j][0] = i;
					}
					else{
						table[i][j][1] = 999;
						table[i][j][0] = 999;
					}
				}
			}
		}	
		
		adj_graph[n1][n2] = cost;
		adj_graph[n2][n1] = cost;
		
		while(count != 0){
			count = distance(node);
		}

		for(i =0; i < node; i++){
			for(j = 0; j < node; j++){
				if(table[i][j][1] != 999)
					fprintf(fwp,"%d %d %d\n", j, table[i][j][0], table[i][j][1]);
			}
			fprintf(fwp,"\n");
		}
		
		message(fwp, mesgfile);
	}
	
	fclose(fp);
	return 0;
}
