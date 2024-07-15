#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORTNUM 9000

int main(void) {
    int sd;
    char buf[256];
    struct sockaddr_in sin;

    // 소켓 생성
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // 소켓 설정
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(PORTNUM);

    // 연결
    if (connect(sd, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("connect");
        exit(1);
    }

    // 읽기
    if (read(sd, buf, sizeof(buf)) == -1) {
        perror("recv");
        exit(1);
    }

    // 끝 & 출력
    close(sd);
    printf("From Server : %s\n", buf);

    return 0;
}
