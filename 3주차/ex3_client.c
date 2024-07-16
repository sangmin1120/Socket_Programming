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
    // ���� init & ���ε�
    int sd;
    sd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in sin;
    memset((char*)&sin,'\0',sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(IP_addr);
    sin.sin_port = htons(PORTNUM);

    // ����
    if (connect(sd,(struct sockaddr*)&sin,sizeof(sin))==-1){
        perror("connection error\n");
        exit(1);
    }

    while(1){
        char buf[256] = {'\0'};
        char content[256] = {'\0'};
        fprintf(stdout,"Input mesasage : ");
        fgets(buf,sizeof(buf),stdin);

        // ������ ������ 
        if (write(sd,buf,sizeof(buf))==-1){
            perror("send erorr\n");
            exit(1);
        }
        // �������� �ޱ�
        if (read(sd,content,sizeof(content))==-1){
            perror("recv error\n");
            exit(1);
        }
        // ���
        fprintf(stdout,"[Text From server : %s]\n",content);
    }
}