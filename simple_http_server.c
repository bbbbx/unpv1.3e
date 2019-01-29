/**
 * 一个非常简单的 HTTP 服务器，
 * 对所有的请求都响应 "<h1>hello world</h1>"。
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define  LISTENQ  1024

int main(void)
{
    int                 listenfd, connfd;
    struct sockaddr_in  servaddr, cliaddr;
    char                request[2048];
    socklen_t           clilen;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(80);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    for (;;) {
        int n, childpid;
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        if ((childpid = fork()) == -1) {
            printf("fork 出错！");
            return -1;
        } else if (childpid == 0) {
            close(listenfd);
            read(connfd, request, sizeof(request));
            printf("%s", request);

            // [TODO] 使用 struct 封装 HTTP 协议格式？
            char   response[] = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Lenght: 20\r\n\r\n<h1>hello world</h1>";
            write(connfd, response, strlen(response));
            exit(0);
        }
        
        close(connfd);
    }
}

/**
 * 编译
 * $ cc ./simple_http_server.c
 */

/**
 * 运行
 * $ ./a.out
 * GET / HTTP/1.1
 * Host: localhost
 * User-Agent: HTTPie/1.0.2
 * Accept-Encoding: gzip, deflate
 * ...
 * Connection: keep-alive
 */

/**
 * 另一个窗口
 * $ curl localhost -i
 * HTTP/1.1 200 OK
 * Connection: close
 * Content-Type: text/html
 * Content-Lenght: 20
 * 
 * <h1>hello world</h1>
 */
