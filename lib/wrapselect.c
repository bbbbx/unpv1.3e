#include "unp.h"

int Select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout)
{
  int n;

  if ((n = select(maxfdp1, readset, writeset, exceptset, timeout)) == -1)
    err_sys("select error");
  else if (n == 0)
    err_sys("select timeout");
  return (n);
}
