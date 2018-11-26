#ifndef     __unp_h

#define     __unp_h

// #include    "config.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>

#ifdef       HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef   HAVE_POLL_H
#include <poll.h>
#endif

#include <sys/event.h>

#include <string.h>

#include <sys/ioctl.h>

#define  LISTENQ  1024
#define  MAXLINE  4096
#define  BUFFSIZE 8192

#define  SERV_PORT        9788              /* TCP and UDP */
#define  SERV_PORT_STR    "9788"            /* TCP and UDP */
#define  UNIXSTR_PATH     "/tmp/unix.str"   /* Unix domain stream */
#define  UNIXDG_PATH      "/tmp.unix.dg"    /* Unix domain stream */

#define  SA  struct sockaddr

int		 Socket(int, int, int);
void   Bind(int, const SA *, socklen_t);
void   Listen(int, int);
int    Accept(int, SA *, socklen_t*);
void   Write(int, void *, size_t);
void   Close(int);

void err_sys(const char *, ...)  __attribute__((noreturn));
void err_quit(const char *, ...) __attribute__((noreturn));

#endif    
