#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <ctype.h>

#define MAXDATASIZE 1000 
int findContentlength(char* content);

int main(int argc, char *argv[]) 
{
	
	// parsing 부분에 대한 변수 
	int i, j = 0;
	char tmp[] = "http://";
	char tmpPort[] = "80";
	char http[MAXDATASIZE] = {'\0'};
	char path[MAXDATASIZE] = {'\0'};	
	char *hostname;
	char *port = NULL;	
	char *tmpHost = NULL;
	char *tmpP = NULL;
	
	// socket 관련 
	int sockfd; 						// socket을 지칭하는 번호 
	int numbytes; 
 	char buf[MAXDATASIZE];
	char tmpbuf[MAXDATASIZE];
 	struct addrinfo hints, *servinfo; 	// socket의 ip, port 담고있는 구조체 
 	int rv; 							// funtional return value
 	char s[INET_ADDRSTRLEN]; 			// ip, address 받기

	// 받은 메세지를 위한 변수
	int byte = 0;
	int contentLength = -1;
	FILE* fwp = fopen("20172067.out", "w");;
	char *statuscode = NULL;
	char status[MAXDATASIZE] = {'\0'};
	char content[MAXDATASIZE] = {'\0'}; 	

        // 입력받은 인수가 정상이 아니라면 프로그램을 종료한다.
        if(argc != 2) {
                fprintf(stderr, "usage: http_client http://hostname[:port][/path/to/file]\n");
                exit(1);
        }
	

	// http:// 부분 있는지 확인 
	for(i = 0; i < strlen(argv[1]); i++){
		if(i < 7 && argv[1][i] != tmp[i]){
 			fprintf(stderr, "usage: http_client http://hostname[:port][/path/to/file]\n"); 
 			exit(1);
		}
		if (i > 6)
			http[j++] = argv[1][i];	
	}
	
	// port, hostname parsing
	tmpHost = strtok(http,"/");
	strcat(path,"/");
	tmpP = strtok(NULL,"");
	if (tmpP != NULL)
		strcat(path,tmpP);

	hostname = strtok(tmpHost,":");
	port = strtok(NULL,"");
	if(port == NULL){
		port = tmpPort;
	}
		 

 	memset(&hints, 0, sizeof hints); 	// 0으로 초기화
 	hints.ai_family = AF_UNSPEC;    	// ipv4 , ipv6, unspecified
 	hints.ai_socktype = SOCK_STREAM;	// tcp(SOCK_STREAM), udp(SOCK_DGRAM)

 	/*  1: ip , 2 : port ,3: ipv4인지 ipv6인지 && tcp인지 udp인지
		4: 정보를 담는다. */  
 	if((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) { 
 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); 
 		return 1; 
	}

	/* create socket :  return -1은 실패한 경우	*/
	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, 
 		servinfo->ai_protocol)) == -1) { 
 		perror("client: socket"); 
 		return 2; // 디버깅 하기위한 장치 
 	} 

 	/* connect to server */
 	if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) { 
 		close(sockfd); 
	 	perror("connect"); 
 		return 3; 
 	}

	// servinfo는 더 이상 필요없기 때문에 free allocated memory for servinfo
	freeaddrinfo(servinfo); 

	// HTTP request message 만들기
	sprintf(buf, "GET %s HTTP/1.1\r\nHost: %s:%s\r\n\r\n",path,hostname,port);

 	// send message  (non-blocking function)
	if(send(sockfd,buf, strlen(buf), 0) == -1) { 
 		perror("send"); 
 		close(sockfd); // socket 닫기 
 		exit(1); 
 	}

	/*  receive message (blocking function) */
	if((numbytes = recv(sockfd, buf, sizeof buf, 0)) == -1){
		perror("recv"); 
		close(sockfd); 
		exit(1); 
	}
	
	buf[numbytes] = '\0';
	strcpy(tmpbuf,buf);
	
	// statsu code parsing 
	statuscode = strtok(tmpbuf,"\r\n");
   	strcpy(status,statuscode);
	
	// data 부분 parsing 
	tmpP = strtok(buf,"");
	tmpHost = strstr(tmpP,"\r\n\r\n");
	fprintf(fwp,"%s",tmpHost+4);
	byte = strlen(tmpHost+4);
	
	// content- length 찾기 
	tmpP = strtok(buf,"\r\n");
    	while(tmpP != NULL){
        	strcpy(content,tmpP);
        	tmpP = NULL;
        	tmpP = strtok(NULL,"\r\n");
        	if(contentLength == -1){
           	 contentLength = findContentlength(content);
        	}
        	memset(content,'\0',1000);
	}
	
	while(contentLength != -1 && contentLength > byte)	{
		numbytes = recv(sockfd, buf, sizeof buf, 0);
		buf[numbytes] = '\0';
		byte += numbytes;
		fprintf(fwp,"%s",buf);
	}	


	// 결과 출력 
	printf("%s\n", status); 
	if(contentLength == -1){
		printf("Content-Length not specified.\n");
		remove("[20172067].out");
	}
	else{
		printf("%d bytes written to 20172067.out\n",contentLength);
	}

	fclose(fwp);
 	close(sockfd); 
	return 0; 
}
int findContentlength(char* content){
	
	int i, j = 0;
	char parsing[] = "content-length";
	char tmp[40] = {'\0',};
	
	for(i = 0; i < strlen(content); i++){
		if(i < strlen(parsing) && tolower(content[i]) != parsing[i]){
			return -1;
		}
		if(i >= strlen(parsing)){
			if(content[i] != ':' && content[i] != ' '){
				tmp[j++] = content[i];
			}
		}

	}

	return strtol(tmp,NULL,10);
}

