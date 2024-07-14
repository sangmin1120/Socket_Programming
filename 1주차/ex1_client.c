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
    int sd, len;
    char buf[256];
    struct sockaddr_un ser;

    if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset((char *)&ser, '\0', sizeof(ser));
    ser.sun_family = AF_UNIX;
    strcpy(ser.sun_path, SOCKET_NAME);
    len = sizeof(ser.sun_family) + strlen(ser.sun_path);

    // 연결
    if (connect(sd, (struct sockaddr *)&ser, len) < 0) {
        perror("connect");
        exit(1);
    }

    // 메세지 보내기
    strcpy(buf, "Unix Domain Socket Test Message");
    if (write(sd, buf, sizeof(buf)) == -1) {
    	perror("send");
    	exit(1);
    }
    // 연결 끊기
    close(sd);

    return 0;
}
