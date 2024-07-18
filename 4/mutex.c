#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
// #include <windows.h>

int a=0;
pthread_mutex_t mutex;


// thread 인자 형태는 맞춰야 됨 , 생성할 때 값을 넘겨준다.
void * thread1(void * arg){

    printf("arg : %d\n",(int)arg);
    while(1){
        pthread_mutex_lock(&mutex);

        printf("thread%d : a[%d]\n",(int)arg,++a);
        pthread_mutex_unlock(&mutex);
        sleep(2);

    }

    return NULL;
} 


int main(){

    // 컨트롤할 thread
    pthread_t s_thread[2];
    int id1 = 77;
    int id2 = 88;

    // mutex 선언
    pthread_mutex_init(&mutex,NULL);

    // discriptor , option , thread 함수 , 넘겨줄 인자
    pthread_create(&s_thread[0],NULL,thread1,(void*)id1);
    pthread_create(&s_thread[1],NULL,thread1,(void*)id2);

    // s_thread를 끝날 때까지 기다려 주는 것이다.
    pthread_join(s_thread[0],NULL);
    pthread_join(s_thread[1],NULL);
}
// gcc 할 때 라이브러리를 참조해주어야 한다. -lthread
// 1. thread 구현
// 2. mutex 구현 : 사이좋게 나눠쓰고 있다. critical section 해결