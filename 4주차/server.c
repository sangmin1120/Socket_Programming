#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CLNT_MAX 10
#define BUFSIZE 200

// cli 부터 받을 discriptor를 배열과 count로 저장
int g_clnt_socks[CLNT_MAX];
int g_clnt_count=0;

// 고객 전용 함수 쓰레드
void * clnt_connection(void *arg){

    // clnt_sock을 가져올 것이다.
    int clnt_sock = (int)arg;
    int str_len=0;

    char msg[BUFSIZE];
    int i;
    while(1){
        if ((str_len=read(clnt_sock,msg,sizeof(msg)))==-1){
            fprintf(stdout,"read error [%d]\n",clnt_sock);
            break;
        } 
        fprintf(stdout,"%s",msg);
    }

    close(clnt_sock);
    pthread_exit(0);
    return NULL;
}

int main(int argc , char ** argv){

    int serv_sock;
    int clnt_sock;
    pthread_t t_thread;

    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    struct sockaddr_in serv_addr;
    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(7989);

    if (bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1){
        perror("bind error");
        exit(1);
    }

    if (listen(serv_sock,5)==-1){
        perror("listen error");
        exit(1);
    }

    char buf[200];
    int recv_len=0;
    while(1){
        // cli 정하기
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);

        // thrad
        pthread_create(&t_thread,NULL,clnt_connection,(void*)clnt_sock);
    }

    return 0;
}