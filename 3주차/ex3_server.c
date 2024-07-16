
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IP_addr "127.0.0.1"
#define PORTNUM 9000

int main(){
    // 소켓 init & 바인딩
    int sd;
    struct sockaddr_in sin,cli;

    if ((sd=socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket create error\n");
        exit(1);
    }

    memset((char*)&sin,'\0',sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(IP_addr);
    sin.sin_port = htons(PORTNUM);

    if (bind(sd,(struct sockaddr *)&sin,sizeof(sin))==-1){
        perror("bind error\n");
        exit(1);
    }

    // listen
    if (listen(sd,5) < 0){
        perror("listen error\n");
        exit(1);
    }
    // accept
    int ns;
    int cli_len=sizeof(cli);
    if ((ns=accept(sd,(struct sockaddr *)&cli,&cli_len))==-1){
        perror("accept error\n");
        exit(1);
    }
    fprintf(stdout,"Wating...\n");

    // communication 
    // q/Q를 받으면 종료 c/C를 받으면 지금까지 받은 문자의 개수
    // cli에서 받은 문자를 ser에서 출력
    int count=0; // 지금가지 받은 문자열의 수
    do{
        char buf[256];
        char content[256];
        // cli에서 문자 읽기
        if (read(ns,buf,sizeof(buf))==-1){
            perror("recv error\n");
            exit(1);
        }
        // 특수 문자 처리
        if (strcmp(buf,"q") || strcmp(buf,"Q"))
            break;
        else if (strcmp(buf,"c") || strcmp(buf,"C")){
            content[0] = (char)count;
            content[1] = '\0';
        }
        // 나머지 케이스 처리 ( 받은 문자열 다시 cli로 보내기 )
        else{
            count++;
            sprintf(content,buf,sizeof(buf)+1);
        }

        // cli로 보내기
        if (write(ns,content,sizeof(content))==-1){
            perror("send error\n");
        }
    }while(1);
    // clsoe
    printf("[Client disconnected]\n");
    close(sd);
    close(ns);
}