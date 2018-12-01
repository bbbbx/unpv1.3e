#include "unp.h"

int Socket(int family, int type, int protocol)
{
  int       n;
  if ((n = socket(family, type, protocol)) < 0)
    err_sys("socket error");
  return(n);
}

void Bind(int fd, const struct sockaddr *sa, socklen_t slen)
{
  if (bind(fd, sa, slen) < 0)
    err_sys("bind error");
}

void Listen(int fd, int backlog)
{
  char   *ptr;

  if ((ptr = getenv("LISTENQ")) != NULL)
    backlog = atoi(ptr);

  if (listen(fd, backlog) < 0)
    err_sys("listen error");
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
  int    n;

again:
  if ((n = accept(fd, sa, salenptr)) < 0) {
  #ifdef EPROTO
    if (errno == EPROTO || errno == ECONNABORTED)
  #else
    if (errno == ECONNABORTED)
  #endif
      goto again;
    else
      err_sys("accept error");
  }
  return (n);
}

void Connect(int family, const struct sockaddr *sa, socklen_t salen)
{
  if (connect(family, sa, salen) < 0)
    err_sys("connect error");
}

int Shutdown(int sockfd, int howto)
{
  if (shutdown(sockfd, howto) == -1)
    err_sys("shutdown erro");
  return (0);
}

int Recvfrom(int sockfd, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen)
{
  int   n;
  
  if ((n = recvfrom(sockfd, buff, nbytes, flags, from, addrlen)) == -1)
    err_sys("recvfrom error");
  return (n);
}

int Sendto(int sockfd, const void *buff, size_t nbytes, int flags, const struct sockaddr *to, socklen_t addrlen)
{
 int   n;
  
  if ((n = sendto(sockfd, buff, nbytes, flags, to, addrlen)) == -1)
    err_sys("sendto error");
  return (n);
}
