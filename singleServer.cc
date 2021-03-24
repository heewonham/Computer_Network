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
/* 동시에 여러사람이 connection request를 보내면
 대기를 하게 되어있다. 그 대기를 몇명까지 받을 것인지를
 설정하는 것. */
#define BACKLOG 10

int main(int argc, char *argv[]) {
 	
 	/*  서버는 기다리는 socket이 있음.
	 	client가 오면 새로운 socket을 만들어서 응대한다.
		즉, clinet 별로 socket을 만든다.  */
	int sockfd; 
	int new_fd;
	
	struct addrinfo hints, *servinfo; 
	struct sockaddr_storage their_addr; // client의 address 저장 
 	socklen_t sin_size; 
 	char s[INET_ADDRSTRLEN]; 
 	int rv; 
 	
 	char buf[MAXDATASIZE]; 
 	int numbytes; 
 	
 	// remote ip 주소가 없기 때문에 port만 받는다.  
 	if(argc != 2) { 
 		printf("usage: server portnum\n"); 
 		exit(1); 
 	}
 	
	memset(&hints, 0, sizeof hints); 
	hints.ai_family = AF_UNSPEC; 
 	hints.ai_socktype = SOCK_STREAM; // tcp 
 	hints.ai_flags = AI_PASSIVE; 	 // server는 기다리는 입장이기 때문에 자신의 IP 사용하겠다는 것. 
 	
 	/* server ip address 없기 때문에 첫 인수를 null로 준다. */
 	if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) { 
 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); 
 		exit(1); 
 	}
 	
 	// create socket 
 	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
	 servinfo->ai_protocol)) == -1) { 
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
	printf("server: waiting for connections on port %s...\n", argv[1]); 
	
 	sin_size = sizeof their_addr;
	// accept가 blocking , client connection을 받을 때까지 기다린다.  
 	new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size); 
 	if(new_fd == -1) { 
		perror("accept"); 
 		exit(1); 
 	} 

	inet_ntop(their_addr.ss_family, &((struct sockaddr_in*)&their_addr)->sin_addr, s, sizeof s); 
	printf("server: got connection from %s\n", s); 
 	
 	// 이 socket은 client를 받고 있는 receptionist이기 때문에 한 클리언트만 받고 종료 
	close(sockfd);
	
	// ------------ 메세지 주고받는 part --------- 
	while(1) {
	 	
	 	// blocking
	 	// numbytes == -1 : 에러, 종료 
 		if((numbytes = recv(new_fd, buf, sizeof buf, 0)) == -1) { 
 			perror("recv"); 
 			close(new_fd); 
 			exit(0); 
 		}
		
		// numbytes == 0 : client가 connection을 종료 
 		if(numbytes == 0) { 
 			close(new_fd); 
 			break; 
 		}
		
		// 제대로 메세지 온 것 
		buf[numbytes] = '\0';
		printf("server: received '%s'\n", buf); 
 
		// 받은 것을 그대로 보내주는 eco server이다.
		// 만약 null문자를 안붙였으면 numbyte 개수를 strlen 으로 입력 
 		if (send(new_fd, buf, strlen(buf), 0) == -1) { 
 			perror("send"); 
 			close(new_fd); 
 			exit(0); 
 		} 
 	}
	
	close(new_fd); 
	return 0;
	
}
