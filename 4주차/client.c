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
void* recv_message(void* arg);

char message[BUFSIZE];

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
    strcpy(id,argv[1]);
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
    
    char chat[2000];
    char msg[2000];

    sprintf(msg,"[%s] : hello world\n",id);

    fprintf(stdout,"while before\n");
    while(1){
        fprintf(stdout,"채팅 입력 : ");
        fgets(chat,sizeof(chat),stdin);
        // fprintf(stdout,"debug : %s\n",chat);
        sprintf(msg,"[%s] : %s\n",id,chat);

        fprintf(stdout,"send : %s",msg);
        write(sock,msg,strlen(msg)+1);
        sleep(1);
    }
    fprintf(stdout,"while end\n");
}