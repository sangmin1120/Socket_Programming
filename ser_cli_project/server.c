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

pthread_mutex_t g_mutex;

// 내 sock 빼고 나머지에게 msg를 보냄
void send_all_clnt(char *msg , int my_sock){

    pthread_mutex_lock(&g_mutex);
    for (int i=0;i<g_clnt_count;i++){

        int other_sock = g_clnt_socks[i];

        if (other_sock != my_sock){
            // fprintf(stdout,"debug send_all_cnt (id,msg) : %d , %s ",other_sock,msg);
            write(other_sock,msg,strlen(msg)+1);
        }
    }
    pthread_mutex_unlock(&g_mutex);
}
// 고객 전용 함수 쓰레드
void * clnt_connection(void *arg){

    // clnt_sock을 가져올 것이다.
    int clnt_sock = *(char*)arg;
    int str_len=0;

    char msg[BUFSIZE];
    int i;
    while(1){
        memset(msg,'\0',BUFSIZE);
        str_len=read(clnt_sock,msg,sizeof(msg));

        fprintf(stdout,"read in sock[%d] : %s",clnt_sock,msg);

        if (str_len==-1){
            fprintf(stdout,"read error [%d]\n",clnt_sock);
            break;
        }

        if (strstr(msg,"exit")!=NULL || strlen<=0)
            break;
        fprintf(stdout,"%s",msg);
        send_all_clnt(msg,clnt_sock);
    }

    fprintf(stdout,"disconnected sock[%d]\n",clnt_sock);

    // critical section : socket 연결 해제
    pthread_mutex_lock(&g_mutex);
        close(clnt_sock);

        for (int i=0;i<g_clnt_count;i++){
            if (clnt_sock==g_clnt_socks[i]){
                for (;i<g_clnt_count;i++)
                    g_clnt_socks[i]=g_clnt_socks[i+1];
                break;
                g_clnt_count--;                
            }
        }
    pthread_mutex_unlock(&g_mutex);
    pthread_exit(0);
    return NULL;
}

int main(int argc , char ** argv){

    int serv_sock;
    int clnt_sock;

    pthread_t t_thread;
    
    pthread_mutex_init(&g_mutex,NULL);

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

        // 고객을 저장
        // critical section
        pthread_mutex_lock(&g_mutex);
        g_clnt_socks[g_clnt_count++] = clnt_sock;
        pthread_mutex_unlock(&g_mutex);
        
        // thrad
        pthread_create(&t_thread,NULL,clnt_connection,(void*)&clnt_sock);
    }

    return 0;
}