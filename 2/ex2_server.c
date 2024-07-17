#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PORTNUM 9000

int main(void) {
    char buf[256];
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);

    // 家南 积己
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // 家南 汲沥
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(PORTNUM);

    // 家南 楷搬
    if (bind(sd, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("bind");
        exit(1);
    }

    if (listen(sd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    if ((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1) {
        perror("accept");
        exit(1);
    }

    // buf俊 涝仿 饶 家南俊 静扁
    sprintf(buf, "Your IP address is %s", inet_ntoa(cli.sin_addr));
    if (write(ns, buf, strlen(buf) + 1) == -1) {
        perror("send");
        exit(1);
    }
    close(ns);
    close(sd);

    return 0;
}
