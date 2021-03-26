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
	int sockfd;
	int numbytes; 
 	char buf[MAXDATASIZE];
 	char str[100]; // 사용자 입력받기 
 	struct addrinfo hints, *servinfo; 
 	int rv;
 	char s[INET_ADDRSTRLEN]; 
 
	if(argc != 3) { 
 		fprintf(stderr, "usage: client hostname portnum\n"); 
 		exit(1); 
 	}	 
 	
 	memset(&hints, 0, sizeof hints); 	// 0으로 초기화
 	hints.ai_family = AF_UNSPEC;    
 	hints.ai_socktype = SOCK_STREAM;	
 	
  	if((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) { 
 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); 
 		return 1; 
	}
	
	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, 
 		servinfo->ai_protocol)) == -1) { 
 		perror("client: socket"); 
 		return 2; 
 	} 
 	
  	if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) { 
 		close(sockfd); 
	 	perror("client: connect"); 
 		return 3; 
 	}
	
 	inet_ntop(servinfo->ai_family, &((struct sockaddr_in*)servinfo->ai_addr)->sin_addr, s, sizeof s); 
 	printf("client: connecting to %s\n", s);
	 
	freeaddrinfo(servinfo); 
 	
 	while(1){
 		printf("command> ");
 		fgets(str,100,stdin);

		if(send(sockfd, str, strlen(str), 0) == -1) { 
 			perror("send"); 
 			close(sockfd); // socket 닫기 
 			exit(1);
 			break;
 	        }

		if((numbytes = recv(sockfd, buf, sizeof buf, 0)) == -1) { 
 			perror("recv"); 
 			close(sockfd); 
			exit(1);
                        break;
                }
		buf[numbytes] = '\0';
                if(!strcmp(buf, "quit")){
 			perror("send"); 
 			close(sockfd); // socket 닫기 
 			exit(1);
 			break;
                }else
                       printf("client: received '%s'\n", buf); 
	 }
 	close(sockfd); 
	return 0; 
}
