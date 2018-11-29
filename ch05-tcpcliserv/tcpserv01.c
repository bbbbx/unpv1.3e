#include "../lib/unp.h"

int main(void)
{
  int                listenfd, connfd;
  struct sockaddr_in servaddr, cliaddr;
  pid_t              childpid;
  socklen_t          clilen;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  for (;;) {
    clilen = sizeof(cliaddr);
    connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

    if ((childpid  = Fork()) == 0) {
      Close(listenfd);   /* child close listening socket */
      str_echo(connfd);  /* process the request */
      exit(0);
    }
    Close(connfd);    /* parent close connected socket */
  }
}
