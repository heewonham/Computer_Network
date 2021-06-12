# computer_network

## Lab 01
- 고객은 서버에 접속하면 서버에게 "hello"를 보내고,
- 서버는 하나의 고객이 보낸 메세지를 그대로 보내준다. (eco server)
- 오진 단 한명의 고객만 받는 single server이다.



## Lab 02

* select 를 활용하여 서버는 여러 명의 client를 받는다.
* client도 select를 활용하여 정해진 대로 수행하는 것이 아닌, 자유롭게 서버에게 정보가 오면  받고, 서버에게 정보를 보내는 작업이 가능하게 한다.




## Assignment 1 - HTTP client 만들기

* Web browser 와 같은 역할을 하는 HTTP client 프로그램 작성한다.
* 서버와 TCP 연결하고 HTTP request를 보낸다. HTTP response를 받아서 헤더의 status code를 화면에 출력하고, header 안에 Content-Length를 기억하여 데이터를 학번.out파일에 작성한다.



## Assignment 2 - Distance Vector , Link State routing algorithm

* Link State routing algorithm을 이용한 라우팅 테이블을 관리하는 프로그램과 Distance Vector routing algorithm을 이용한 라투이 테이블 관리하는 프로그램을 작성한다.

* ./linkstate topologyfile messagesfile changesfile
  ./distvec topologyfile messagesfile changesfile

  3가지 인자를 받아 프로그램을 실행시킨다.

  

1 ) topologyfile : 노드들의 정보를 읽어와 라우팅 테이블을 완성하고

2 ) messagesfile : 파일 안에 출발 노드와 도착 노드를 어떤 경로로 연결하여 전달할 것인지를 출력한다. 

3 ) changesfile : 변경한 노드 정보를 수정하고 라우팅 테이블을 업데이트하여 messagesfile 의 출발 노드와 도착 노드를 어떤 경로로 전달할 것인지 출력한다.



## Assignment 3 - Cyclic Redundancy Check

* crc_encoder: CRC를 이용하여 dataword를 codeword로 변환한다.
* crc_decoder: codeword를 dataword로 복원하면서, 오류가 있으면 검출한다.



1 ) crc_encoder

​	/crc_encoder input_file output_file generator dataword_size 

​	4가지 인자를 받아 프로그램을 실행시킨다.

* input_file: 전송하고자 하는 파일
- output_file: 전송하고자 하는 파일을 CRC로 encode한 파일
- generator: CRC의 generator (최대 길이는 9로 지정하며, generator의 MSB는 항상 1이다.)
* dataword_size: dataword의 크기. crc_encoder에서는 파일을 dataword의 크기로 나누어 각 dataword를 codeword로 변화시켜야 한다. 단위는 비트(bit)이다



실행방법 :

 dataword를 modulo-2 division을 수행해서 나머지를 구하고, 이를 dataword에 합쳐 codeword를 만든다. 

- 프로그램의 인자로 주어진 generator가 '1101'이라고 가정한다.
- 첫번째 dataword는 '0100'이다.
- generator가 네 자리이므로, dataword 뒤에 '000'을 붙여준다.
- modulo-2 나눗셈을 이용하여 '0100000'을 '1101'로 나눠준다

CRC를 이용하여 변환한 codeword들을 output file에 바이트 단위로 쓰기 위해서 padding bit를 계산한다. padding bit를 첫 바이트로 작성 후 padding bit와 codeword를 붙여서 output file에 write 해준다.  



2 ) crc_decoder

​	./crc_decoder input_file output_file result_file generator dataword_si

​	5개의 인자를 받아 프로그램을 실행시킨다.

- input_file: CRC로 encode된 파일
- output_file: CRC를 제거하고 원래 데이터 파일을 복원한 파일
- result_file: 전체 프레임의 수와 오류가 난 프레임의 수를 표시할 파일
- generator: CRC의 generator
- dataword_size: dataword의 크기. 단위는 비트(bit)이다.



실행방법 : 

1. 첫번째 바이트를 읽어서, 패딩 bit 개수를 확인한다.
2. 그 다음 바이트에서 패딩 bit를 제거하고, 그 이후 비트를 codeword 크기로 나누어, 각 codeword에 대해서 generator를 이용하여 modulo-2 나눗셈을 함으로써 codeword에 오류가 있는지 확인한다. 전체 codeword의 개수와 오류가 난 codeword의 개수를 기록한다.
3.  codeword에 오류가 있든 없든 dataword로 복원하여 출력 파일에 쓴다.
4.  result file에는 총 codeword 개수와 오류가 난 codeword의 개수를 기록한다.



-------



