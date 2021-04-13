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

#define MAXDATASIZE 1000 

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
 	
	
	// http:// 부분 있는지 확인 
	for(i = 0; i < strlen(argv[1]); i++){
		if(i < 7 && arg[1][i] != tmp[i]){
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
	
	// 입력받은 인수가 정상이 아니라면 프로그램을 종료한다. 
 	if(argc != 2) { 
 		fprintf(stderr, "usage: http_client http://hostname[:port][/path/to/file]\n"); 
 		exit(1); 
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
	sscanf(tmpbuf, "GET %s HTTP/1.1\r\n",path);
	strcpy(buf,tmpbuf);
	sscanf(tmpbuf, "Host: %s:%s\r\n",hostname,port);
	strcat(buf,tmpbuf);
	strcat(buf,"\r\n\r\n");

 	// send message  (non-blocking function)
	if(send(sockfd, buf, strlen(buf), 0) == -1) { 
 		perror("send"); 
 		close(sockfd); // socket 닫기 
 		exit(1); 
 	}

	/*  receive message (blocking function) */
	if((numbytes = recv(sockfd, buf, sizeof buf, 0)) == -1) { 
 		perror("recv"); ./
 		close(sockfd); 
		exit(1); }
	buf[numbytes] = '\0';
	// 메세지가 올때 null문자가 와서 오지 않기 때문에 null 문자를 끝에 붙인다.
 	printf("client: received '%s'\n", buf); 

 	close(sockfd); 
	return 0; 
}
