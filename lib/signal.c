#include "unp.h"

Sigfunc *Signal(int signo, Sigfunc *func)
{
  Sigfunc *sigfunc;

  if ((sigfunc = signal(signo, func)) == SIG_ERR)
    err_sys("signal error");
  return (sigfunc);
}
