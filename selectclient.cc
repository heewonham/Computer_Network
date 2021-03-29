/*      클라이언트도 select 하는 이유 
        타이핑하는 것과 메세지를 받는 것(in : 0)을  
	나누기 위해서 */ 
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

#define MAXDATASIZE 1000 // max number of bytes we can get at once 

int main(int argc, char *argv[]) 
{ 

	int sockfd, numbytes; 
	char buf[MAXDATASIZE]; 
	struct addrinfo hints, *servinfo; 
 	int rv; 
 	char s[INET_ADDRSTRLEN]; 
 	
 	// 추가 
 	fd_set master;
	fd_set read_fds;
	
	if (argc != 3) { 
 		fprintf(stderr,"usage: %s hostname portnum\n", argv[0]); 
 		exit(1); 
 	}
	
	memset(&hints, 0, sizeof hints); 
	hints.ai_family = AF_UNSPEC; 
 	hints.ai_socktype = SOCK_STREAM; 
 	
 	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); 
 		return 1; 
 	}
	
	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) { 
 		perror("client: socket"); 
 		return 2; 
 	}
	  
 	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) { 
 		close(sockfd); 
 		perror("client: connect"); 
 		return 2; 
 	}
	 
	inet_ntop(servinfo->ai_family, &((struct sockaddr_in*)servinfo->ai_addr)->sin_addr, s, sizeof s); 
	printf("client: connecting to %s\n", s); 
	
	freeaddrinfo(servinfo); // all done with this structure 
	
	FD_ZERO(&master); 
	FD_SET(0, &master); // 0 : stdin을 말함. 
	FD_SET(sockfd, &master); 
	
	int quit = 0; 
	while(1) { 
		read_fds = master; 
		if(select(sockfd+1, &read_fds, NULL, NULL, NULL) == -1) { 
 			perror("select"); 
 			exit(1); 
 		}
		
		// 사실 sockfd가 최대 번호이기 때문에 sockfd+1함.
		// 0, 3번만 봐도 됨. 여튼 0~3가 있기 때문에 +1 붙임  
 		for(int i=0; i<=sockfd+1; i++) { 
 			if(FD_ISSET(i, &read_fds)) { 
 				// 1. 유저가 메세지를 넣은 것. 
				if(i == 0) {
					fgets(buf, sizeof buf, stdin);
	                                buf[strlen(buf)-1] = '\0';
					if(strcmp(buf, "quit") == 0) { 
						quit = 1; 
						break; 
					}
					if(send(sockfd, buf, strlen(buf), 0) == -1) { 
						perror("send"); 
						close(sockfd); 
 						exit(1); 
 					}
				}
				// 2. 서버로부터 온 경우  
				else if(i == sockfd) {
 					if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { 
 						perror("recv"); 
						close(sockfd); 
						exit(1); 
 					} 
 					// server가 연결끊은 경우 
					if(numbytes == 0) { 
						printf("server disconnected.\n"); 
						quit = 1; 
						break; 
					}
					// 정상으로 받은 경우 : 화면에 찍기 
					buf[numbytes] = '\0'; 
					printf("%s\n", buf); 
 				} 
			} 
 		} 
 		if(quit) break; 
 	} 
 	printf("connection closed.\n"); 
 	close(sockfd); 
 	return 0; 
} 
