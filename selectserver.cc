#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <arpa/inet.h>

#define MAXDATASIZE 1000 
#define BACKLOG 10 

/*  비동기적으로 메시지를 보내고 싶을 때 보내고 
	받고 싶을 때는 받는 형태의 server 이다. 
	select 함수를 사용하는 것은 하나의 쓰레드에서 관리를 하기 때문
	서버를 만들 수 있는 방법이 multi-process, thread 등 여러개 있고
	그 중에  select는 여러 방법 중 하나이다.*/ 
int main(int argc, char *argv[]) { 
	
	int sockfd; 
	int newfd; 
	
	struct addrinfo hints, *servinfo; 
	struct sockaddr_storage their_addr; 
	socklen_t sin_size; 
 	char s[INET_ADDRSTRLEN]; 
 	int rv;
 	
 	// file description set 
 	// fd : 소켓의 집합을 위한 자료구조 
 	fd_set master; 
 	fd_set read_fds; 
	int fdmax; // 소켓 번호의 최대를 기억 
	
 	char buf[MAXDATASIZE];
	int numbytes; 
	if(argc != 2) { 
		printf("usage: server portnum\n"); 
 		exit(1); 
 	}
	
	memset(&hints, 0, sizeof hints); 
 	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM; 
 	hints.ai_flags = AI_PASSIVE; // use my IP 
 	
	if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) { 
 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); 
 		exit(1); 
 	}
	 
 	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) { 
		perror("server: socket"); 
 		exit(1); 
 	}
	 
	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) { 
 		close(sockfd); 
 		perror("server: bind"); 
		exit(1); 
 	}
	
	freeaddrinfo(servinfo); 

	if (listen(sockfd, BACKLOG) == -1) { 
 		perror("listen"); 
 		exit(1); 
 	}
	
	// accept를 하기전에 수행
	FD_ZERO(&master); //master file description을 만들었을 때 초기상황을 몰라서 모두 꺼주는 역할 
	FD_SET(sockfd, &master); // activate을 시킨다. 
	fdmax = sockfd; 
	
	while(1) { 
		
		read_fds = master; 
		// 최대 + 1, 확인하고자하는 fd set, 
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) { 
 			perror("select"); 
			exit(1); 
 		}
 		
 		// 소켓이 불이 켜져있는 지 확인
		// 불이 켜져 있는 것은 메세지가 도착함을 의미   
		for(int i=0; i<=fdmax; i++) { 
			if(FD_ISSET(i, &read_fds)) { 
				/*socket이 만들어졌을 때 고객을 맞이하는 socket임.
				맨 처음 receiptional socket
				그러므로 이것이 켜져있으면 client의 connection 연결이 온것*/
				if(i == sockfd) { 
					sin_size = sizeof their_addr; 
 					newfd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size); 
					if(newfd == -1) { 
 						perror("accept"); 
 					} else { 
						FD_SET(newfd, &master); // master 갱신 
 						if(newfd > fdmax) {  fdmax = newfd; } 
						printf("selectserver: new connection from %s on socket %d\n", 
 						inet_ntop(their_addr.ss_family, &((struct sockaddr_in*)&their_addr)->sin_addr, s, sizeof s), newfd); 
 					} 
 				}
				/* 클라이언트를 상대하는 socket에게 연락이 온 것*/
				else { 
					// -1: 에러 , 0: 클라이언트 언결 종료, 양수: 메세지 수 
 					if((numbytes = recv(i, buf, sizeof buf, 0)) <= 0) { 
						if(numbytes == 0) { 
 							printf("selectserver: socket %d hung up\n", i); 
						}else { 
							perror("recv"); 
 						} 
						close(i); 
 						FD_CLR(i, &master); // 마스터에서 불꺼줌 
					}
					else { 
						buf[numbytes] = '\0'; 
						printf("server received: %s\n", buf); 	
						if(send(i, buf, strlen(buf), 0) == -1) { 
 							perror("send"); 
 						} 
 					} 
 				} 
			} 
 		} 
 	} 
}
