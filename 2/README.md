# 2주차
---------
AF_UN 대신 AF_INET을 사용하였다.

AF_UN은 프로그램 간 통신 이기 때문에 ip , protnumber가 필요하지 않았다.

AF_INET은 인터넷 통신하는 소켓이기 때문에 ip , portnumber가 필요하다

ip는 컴퓨터가 1대라서 loop back 번호를 사용했다.

portnumber는 고정적이지 않은 portnumber를 server와 client에 같이 적용해 주었다.

어떤 방식인지는 감이 오지만 코드를 짤 때 매개변수 설정이 좀 오래 걸린다.
