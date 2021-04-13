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
	
	// parsing �κп� ���� ���� 
	int i, j = 0;
	char tmp[] = "http://";
	char tmpPort[] = "80";
	char http[MAXDATASIZE] = {'\0'};
	char path[MAXDATASIZE] = {'\0'};	
	char *hostname;
	char *port = NULL;	
	char *tmpHost = NULL;
	char *tmpP = NULL;
	
	// socket ���� 
	int sockfd; 						// socket�� ��Ī�ϴ� ��ȣ 
	int numbytes; 
 	char buf[MAXDATASIZE];
 	char tmpbuf[MAXDATASIZE]; 
 	struct addrinfo hints, *servinfo; 	// socket�� ip, port ����ִ� ����ü 
 	int rv; 							// funtional return value
 	char s[INET_ADDRSTRLEN]; 			// ip, address �ޱ�
 	
	
	// http:// �κ� �ִ��� Ȯ�� 
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
	
	// �Է¹��� �μ��� ������ �ƴ϶�� ���α׷��� �����Ѵ�. 
 	if(argc != 2) { 
 		fprintf(stderr, "usage: http_client http://hostname[:port][/path/to/file]\n"); 
 		exit(1); 
 	}	 

 	memset(&hints, 0, sizeof hints); 	// 0���� �ʱ�ȭ
 	hints.ai_family = AF_UNSPEC;    	// ipv4 , ipv6, unspecified
 	hints.ai_socktype = SOCK_STREAM;	// tcp(SOCK_STREAM), udp(SOCK_DGRAM)

 	/*  1: ip , 2 : port ,3: ipv4���� ipv6���� && tcp���� udp����
		4: ������ ��´�. */  
 	if((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) { 
 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); 
 		return 1; 
	}

	/* create socket :  return -1�� ������ ���	*/
	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, 
 		servinfo->ai_protocol)) == -1) { 
 		perror("client: socket"); 
 		return 2; // ����� �ϱ����� ��ġ 
 	} 

 	/* connect to server */
 	if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) { 
 		close(sockfd); 
	 	perror("connect"); 
 		return 3; 
 	}

	// servinfo�� �� �̻� �ʿ���� ������ free allocated memory for servinfo
	freeaddrinfo(servinfo); 

	// HTTP request message �����
	sscanf(tmpbuf, "GET %s HTTP/1.1\r\n",path);
	strcpy(buf,tmpbuf);
	sscanf(tmpbuf, "Host: %s:%s\r\n",hostname,port);
	strcat(buf,tmpbuf);
	strcat(buf,"\r\n\r\n");

 	// send message  (non-blocking function)
	if(send(sockfd, buf, strlen(buf), 0) == -1) { 
 		perror("send"); 
 		close(sockfd); // socket �ݱ� 
 		exit(1); 
 	}

	/*  receive message (blocking function) */
	if((numbytes = recv(sockfd, buf, sizeof buf, 0)) == -1) { 
 		perror("recv"); ./
 		close(sockfd); 
		exit(1); }
	buf[numbytes] = '\0';
	// �޼����� �ö� null���ڰ� �ͼ� ���� �ʱ� ������ null ���ڸ� ���� ���δ�.
 	printf("client: received '%s'\n", buf); 

 	close(sockfd); 
	return 0; 
}
