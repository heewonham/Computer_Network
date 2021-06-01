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



