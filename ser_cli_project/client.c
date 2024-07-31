#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFSIZE 100
#define NAMESIZE 20

int StoHEX(char fi , char sc);
void error_handling(char *msg);
void* send_message(void* arg);

char message[BUFSIZE];

// 서버에서 메시지를 받을 스레드
void* recv_msg(void *arg){

    int sock = *(int*)arg;

    char buf[BUFSIZE];
    int len;
    while(1){
        len=read(sock,buf,sizeof(buf)+1);
        if (len==-1){
            printf("sock close\n");
            break;
        }

        fprintf(stdout,"%s",buf);
    }

    pthread_exit(0);
    return NULL;
}
int main(int argc , int **argv){

    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread , rcv_thread;
    void* thread_result;

    char id[100];

    // argv가 안들어옴
    if (argc<=1){
        fprintf(stdout,"you have to enter ID\n");
        return 0;
    }
    strcpy(id,(char*)argv[1]);
    printf("id : %s\n",id);

    sock=socket(PF_INET , SOCK_STREAM , 0);
    if (sock==-1)
        perror("socket error");
    
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(7989);

    if (connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        perror("connect error");
    else
        fprintf(stdout,"connection success\n");
    
    if (pthread_create(&rcv_thread,NULL,recv_msg,(void*)&sock) != 0)
        perror("pthread_create error");
    else
        fprintf(stdout,"rcv thread created\n");

    char chat[1500];
    char msg[2000];

    sprintf(msg,"[%s] : hello world\n",id);

    fprintf(stdout,"while before\n");
    while(1){
    // 보내는 부분
        fprintf(stdout,"채팅 입력 : ");
        fgets(chat,sizeof(chat),stdin);
        sprintf(msg,"[%s] : %s\n",id,chat);
        
        fprintf(stdout,"send : %s",msg);
        write(sock,msg,strlen(msg)+1);
        
        if (strcmp(chat,"exit\n")==0)
            break;

        sleep(1);
    }
    fprintf(stdout,"while end\n");
}