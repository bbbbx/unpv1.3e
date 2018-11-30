#include "unp.h"

int Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
  int n;

  if ((n = poll(fdarray, nfds, timeout)) == -1)
    err_sys("poll error");
  else if (n == 0)
    err_sys("poll timeout");
  return (n);
}
