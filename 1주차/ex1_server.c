#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SOCKET_NAME "kssocket"

int main(void) {
    // 소켓 생성
    char buf[256];
    struct sockaddr_un ser, cli;
    int sd, nsd, len, clen;

    unlink(SOCKET_NAME);

    if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset((char *)&ser, 0, sizeof(struct sockaddr_un));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    // 연결
    if (bind(sd, (struct sockaddr *)&ser, len)) {
        perror("bind");
        exit(1);
    }

    // 클라이언트 요청 대기시킬 수신 대기열
    if (listen(sd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    // 대기열에서 연결 요청을 확인
    printf("Waiting ...\n");
    clen = sizeof(cli);
    if ((nsd = accept(sd, (struct sockaddr *)&cli, &clen)) == -1) {
        perror("accept");
        exit(1);
    }

    // 메세지 받기
    if (read(nsd, buf, sizeof(buf)) == -1) {
        perror("recv");
        exit(1);
    }

    printf("Received Message: %s\n", buf);
    close(nsd);
    close(sd);

    return 0;
}
