// 꼭 필요한 구조체 
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

// 버퍼의 사이즈 
#define MAXDATASIZE 1000 

int main(int argc, char *argv[]) 
{
	int sockfd; // socket을 지칭하는 번호 
	int numbytes; 
 	char buf[MAXDATASIZE]; 
 	struct addrinfo hints, *servinfo; // socket의 ip, port 담고있는 구조체 
 	int rv; // funtional return value
 	char s[INET_ADDRSTRLEN]; // ip, address 받기 
 
	// 입력받은 인수가 정상이 아니라면 프로그램을 종료한다. 
 	if(argc != 3) { 
 		fprintf(stderr, "usage: client hostname portnum\n"); 
 		exit(1); 
 	}	 
 	
 	memset(&hints, 0, sizeof hints); 	// 0으로 초기화
 	hints.ai_family = AF_UNSPEC;    	// ipv4 , ipv6, unspecified
 	hints.ai_socktype = SOCK_STREAM;	// tcp(SOCK_STREAM), udp(SOCK_DGRAM)
 	
 	/*  첫번째 인수는 : ip
	 	두번째 인수는 : port number
		세번째 인수는 : ipv4인지 ipv6인지 && tcp인지 udp인지
		네번째 인수에다가 정보를 담는다.
		return value는 제대로 입력되었는지를 확인한다.
		0이 아니면, 에러가 난것을 의미함.  */  
 	if((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) { 
 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv)); 
 		return 1; 
	}
	
	/* create socket
	   return -1은 실패한 경우	*/
	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, 
 		servinfo->ai_protocol)) == -1) { 
 		perror("client: socket"); 
 		return 2; // 디버깅 하기위한 장치 
 	} 
 	
 	/* connect to server */
 	if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) { 
 		close(sockfd); 
	 	perror("client: connect"); 
 		return 3; 
 	}
	
	// print to screen : 화면에 address를 string으로 바꿔 출력
	// connection 과는 상관 없다.  
 	inet_ntop(servinfo->ai_family, &((struct sockaddr_in*)servinfo->ai_addr)->sin_addr, s, sizeof s); 
 	printf("client: connecting to %s\n", s);
	 
	// servinfo는 더 이상 필요없기 때문에 free allocated memory for servinfo
	freeaddrinfo(servinfo); 
 	
 	/* connection 이후 part 2 부분이다.*/
 	// send message  (non-blocking function)
 	strcpy(buf, "hello"); 
	if(send(sockfd, buf, strlen(buf), 0) == -1) { 
 		perror("send"); 
 		close(sockfd); // socket 닫기 
 		exit(1); 
 	}
	
	/*  receive message (blocking function)
		소켓을 통해서 메세지를 받을 때까지 blocking한다.
		메세지를 주고 받기 위해서는 여러 방법이 있음.	*/
	if((numbytes = recv(sockfd, buf, sizeof buf, 0)) == -1) { 
 		perror("recv"); 
 		close(sockfd); 
		exit(1); }
	buf[numbytes] = '\0';
	// 메세지가 올때 null문자가 와서 오지 않기 때문에 null 문자를 끝에 붙인다.
 	printf("client: received '%s'\n", buf); 
 	
 	close(sockfd); 
	return 0; 
}
