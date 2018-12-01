#include "unp.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define    BUFSIZE   1500

char      sendbuf[BUFSIZE];
int       datalen;  /* bytes of data, following ICMP header */
char      *host;
int       nsent;    /* 每次 sendto() 就加一 */
pid_t     pid;      /* our PID */
int       sockfd;
int       verbose;

void      int_v6(void);
void      proc_v4(char *, ssize_t, struct msghdr *, struct timeval *);
void      proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void      send_v4(void);
void      send_v6(void);
void      readloop(void);
void      sig_alrm(int);
// void      tv_sub(struct timeval *, struct timeval *);

struct proto {
  void      (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
  void      (*fsend)(void);
  void      (*finit)(void);
  struct sockaddr    *sasend;         /* sockaddr{} for sending, from getaddrinfo */
  struct sockaddr    *sarecv;         /* sockaddr{} for receiving */
  socklen_t          salen;           /* sockaddr{}s 的 length */
  int                icmpproto;       /* IPPROTO_XXX value fro ICMP */
} *pr;

#ifdef   IPV6
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#endif
