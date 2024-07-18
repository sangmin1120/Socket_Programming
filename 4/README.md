# socket programming 채팅방 만들기


## thread : 프로세스 내에서 실행되는 그 안의 작은 프로세스라고 생각하면 된다.
-----------------------------------------------------------------------------
- thread.c

#include <pthread.h>를 해줘야한다.

2개의 thread를 선언하고 pthread_create로 만들어 주었다. => 매개 변수 : disciriptor  , option(NULL) , thread 함수 , 함수에 넘겨줄 인자

그리고 종료하면 pthread_join을 호출하면 된다.

원래 프로세스 같으면 함수를 호출해 무한 루프를 돌고있지만 ,

2개의 thread를 사용했기 때문에 thread[0] 이 끝나면 thread[1]이 실행되는 것이다.

참고) 실행 파일을 생성할 때 gcc -o thread thread.c -lpthread , 헤더를 포함해 줘야한다.



## mutex : thread를 사용할 때 critical section ( 중요한 자원 ) 을 번갈아 가며 사용하기 위해 사용되는 구조이다.
----------------------------------------------------------------------
- mutex.c

pthread_mutex_t mutex; 로 선언해 준다. 웬만하면 전역으로 선언한다.

critical section 전 후에 pthread_mutex_lock(&mutex,NULL) , pthread_mutex_unlock(&mutex) 를 호출하면 된다.

thread.c 에서는 공유하는 자원 (a) 를 동시에 사용하는 경우의 수가 발생했지만 , mutex는 그럴 일이 전혀 없다.



## server.c / client.c
--------------------------------
- 기능 : 채팅방
  
  1. client와 server를 연결한다.
  2. 여러 client들이 연결할 수 있다.
  3. 한 client가 server로 메시지를 보내면
  4. 연결되어 있는 client들이 메세지를 확인할 수 있다.
  5. client에서 'exit'을 입력하면 서버와의 연결을 해제할 수 있다.

### 코드 흐름
- server.c
  
  main에서 소켓을 연결한다.
  
  listen으로 연결되어있는 소켓을 배열에 저장하고 accept해서 한 개씩 가져온다.

  무한 루프 : 소켓을 한 개씩 가져와서 배열에 저장

    clnt_connection thread 만든다.

  clnt_connection : client 한 명의 socket에서 무한 루프로 문자열을 계속 받는다. 받자 마자 => send_all_clnt : 연결되어있는 배열을 이용해 모든 사용자에게 메시지를 보낸다.

  socket을 해제할 때는 공유 자원 (g_clnt_socks)에 접근하기 때문에 mutex를 사용해 주어야한다.


- client.c
  
  main에서 소켓을 연결한다.

  recv thread 생성 : 서버의 socket을 무한 루프에서 서버에서 보내는 메시지(즉, 다른 사람이 보내는 메시지)를 읽어온다.

  무한 루프에서 채팅을 입력받고 받은 채팅을 서버로 보내준다.


- 이용 방법
  
    : servevr.c 와 client.c Makefile을 설치한다.

    리눅스 환경에서 make client 와 make server 명령어를 실행하면 실행 파일이 만들어진다.

    ./server 를 실행하고 ./client {id} 를 입력해 id는 서로를 구분해주는 이름이다.

  여러 ./client {id}를 실행해 서로 채팅을 할 수 있다.
  





  
