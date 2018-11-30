/* Our own header. */

#ifndef     __unp_h

#define     __unp_h

#include    "config.h"    /* configuration options for current OS，该文件由 configure 生成 */

#include <sys/types.h>     /* basic system data types */
#include <sys/socket.h>    /* basic socket definitions */
#include <sys/time.h>      /* timeval{} for select() */
#include <time.h>          /* timespec{} for pselect() */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>     /* inet(3) functions */
#include <errno.h>
#include <fcntl.h>         /* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>       /* for S_xxx file mode constants */
#include <sys/uio.h>        /* for iovec{} and readv/writev */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>         /* for Unix domain sockets */

#ifdef       HAVE_SYS_SELECT_H
  #include <sys/select.h>
#endif

#ifdef   HAVE_POLL_H
  #include <poll.h>
#endif

#ifdef   HAVE_SYS_EVENT_H
  #include <sys/event.h>    /* for kqueue */
#endif

#ifdef   HAVE_STRING_H
  #include <string.h>
#endif

/* Three headers are normally needed for socket/file ioctl's: <sys/ioctl.h>, <sys/filio.h>, <sys/sockio.h> */
#ifdef   HAVE_SYS_IOCTL_H
  #include <sys/ioctl.h>
#endif
#ifdef   HAVE_SYS_FILIO_H
  #include <sys/filio.h>
#endif
#ifdef   HAVE_SYS_SOCKIO_H
  #include <sys/sockio.h>
#endif

#ifdef   HAVE_PTHREAD_H
  #include <pthread.h>
#endif

#ifdef   HAVE_NET_IF_DL_H
  #include <net/if_dl.h>
#endif

/* 是否支持 SCTP，Mac OS X 10.14 上没有这个头文件 */
#ifdef   HAVE_NETINET_SCTP_H
  #include <netinet/sctp.h>      
#endif

/* OSF/1 actually disable recv() and send() in <sys/socket.h> */
#ifdef   __osf__
  #undef   recv
  #undef   send
  #define  recv(a,b,c,d)    recvfrom(a,b,c,d,0,0)
  #define  send(a,b,c,d)    sendto(a,b,c,d,0,0)
#endif

#ifndef   INADDR_NONE      /* 应该在 <netinet/in.h> 头文件中有定义 */
  #define  INADDR_NONE   0xffffffff
#endif

#ifndef   SHUT_RD
  #define   SHUT_RD    0
  #define   SHUT_WR    1
  #define   SHUT_RDWR  2
#endif

#ifndef   INET_ADDRSTRLEN           /* "ddd.ddd.ddd.ddd\0" */
  #define   INET_ADDRSTRLEN     16    /* 1234567890123456 */
#endif

/* Define following even if IPv6 not supported, so we can alway allocate
   an adequately sized buffer without #ifdefs in th code. */
#ifndef   INET6_ADDRSTRLEN
  #define   INET6_ADDRSTRLEN    46   /* IPv6 address string 的最大 size:
                                        "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx" 或者
                                        "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd\0"
                                        1234567890123456789012345678901234567890123456 */
#endif

#ifndef   HAVE_BZERO
  #define   bzero(ptr, n)    memset(ptr, 0, n)
#endif

#ifndef   HAVE_GETHOSTBYNAME2
  #define   gethostbyname2(host, family)    gethostbyname((host))
#endif

/* 由 recvfrom_flags() 函数返回的 structure */
struct unp_in_pktinfo {
  struct in_addr   ipi_addr;      /* dst IPv4 address */
  int              ipi_ifindex;   /* received interface index */
};

/* 我们需要两个新的 CMSG_LEN() 和 CMSG_SPACE() macros，
   但目前很少有 implementations 支持这两个 macros。
   这两个 macros 需要一个 ALIGN() macro，但每个 implementations
   对它的定义都不同 */
#ifndef   CMSG_LEN
  #define   CMSG_LEN(size)    (sizeof(struct cmsghdr)+(size))
#endif
#ifndef   CMSG_SPACE
  #define   CMSG_SPACE(size)  (sizeof(struct cmsghdr)+(size))
#endif

#ifndef   SUN_LEN
  #define   SUN_LEN(su) \
              (sizeof(*(su)) - sizeof((su)->sun_path) + strlen((su)->sun_path))
#endif

#ifndef   AF_LOCAL
  #define   AF_LOCAL     AF_UNIX
#endif
#ifndef   PF_LOCAL
  #define   PF_LOCAL     PF_UNIX
#endif

#ifndef   INFTIM
  #define   INFTIM   (-1)    /* infinite poll timeout */
  #ifdef    HAVE_POLL_H
    #define   INFTIME_UNPH   /* tell unpxti.h we defined it */
  #endif
#endif

#define  LISTENQ  1024
#define  MAXLINE  4096
#define  BUFFSIZE 8192

#define  SERV_PORT        9788              /* TCP and UDP */
#define  SERV_PORT_STR    "9788"            /* TCP and UDP */
#define  UNIXSTR_PATH     "/tmp/unix.str"   /* Unix domain stream */
#define  UNIXDG_PATH      "/tmp.unix.dg"    /* Unix domain stream */

#define  SA  struct sockaddr

#define HAVE_STRUCT_SOCKADDR_STORAGE
#ifndef HAVE_STRUCT_SOCKADDR_STORAGE
/**
 *  RFC 3493: protocol-independent placeholder for socket addresses
 */
  #define __SS_MAXSIZE        128
  #define __SS_ALIGNSIZE      (sizeof(int64_t))
  #ifdef  HAVE_SOCKADDR_SA_LEN
    #define __SS_PAD1SIZE  (__SS_ALIGNSIZE - sizeof(u_char) - sizeof(sa_family_t))
  #else
    #define __SS_PAD1SIZE  (__SS_ALIGNSIZE - sizeof(sa_family_t))
  #endif
  #define __SS__PAD2SIZE  (__SS_MAXSIZE - 2*__SS_ALIGNSIZE)

  struct sockaddr_storage {
  #ifdef HAVE_SOCKADDR_SA_LEN
    u_char    ss_len;
  #endif
    sa_family_t     ss_family;
    char            __ss_pad1[__SS_PAD1SIZE];
    char            __ss_pad2[__SS__PAD2SIZE];
    int64_t         __ss_align;
  };
#endif

#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_MODE        (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void Sigfunc(int);       /* for signal handlers */

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

// #ifndef HAVE_ADDRINFO_STRUCT
//   #include "../lib/addrinfo.h"
// #endif

#ifndef HAVE_IF_NAMEINDEX_STRUCT
  struct if_nameindex {
    unsigned int     if_index;    /* 1, 2, ... */
    char             *if_name;    /* null-terminated name: "le0", ... */
  };
#endif

#ifndef HAVE_TIMESPEC_STRUCT
  struct timespec {
    time_t   tv_sec;
    long     tv_nsec;
  };
#endif

/**
 * lib/wrapsock.c
 */
int		 Socket(int, int, int);               /* Section 1.4 */
void   Bind(int, const SA *, socklen_t);
void   Listen(int, int);
int    Accept(int, SA *, socklen_t*);
void   Connect(int, const SA *, socklen_t);
int    Shutdown(int, int);

/**
 * lib/wrapunix.c
 */
pid_t  Fork(void);                          
ssize_t   Read(int, void *, size_t);
void   Write(int, void *, size_t);
void   Close(int);

void str_echo(int);      /* Section 5.3 */
void str_cli(FILE *, int);  /* Section 5.5 */

char *sock_ntop(const struct sockaddr *, socklen_t);   /* Section 3.8 */
char *Sock_ntop(const struct sockaddr *, socklen_t);

ssize_t readn(int, void *, size_t);          /* Section 3.9 */
ssize_t Readn(int, void *, size_t);

ssize_t writen(int, const void *, size_t);   /* Section 3.9 */
void    Writen(int, const void *, size_t);

ssize_t readline(int, void *, size_t);       /* Section 3.9 */
ssize_t Readline(int, void *, size_t);
ssize_t readlinebuf(void **);                /* Section 3.9 */

/**
 * lib/wrapinet.c
 */
const char *Inet_ntop(int, const void *, char *, size_t);   /* Section 3.7 */
void Inet_pton(int, const char*, void *);

/**
 * lib/wrapstdio.c
 */
char *Fgets(char *, int, FILE *);
void Fputs(const char *, FILE *);

/**
 * lib/wrapselect.c
 */
int Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);

/**
 * lib/wrappoll.c
 */
int Poll(struct pollfd *, unsigned long, int);

void err_sys(const char *, ...)  __attribute__((noreturn));
void err_quit(const char *, ...) __attribute__((noreturn));

#endif    
