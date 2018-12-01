#include "unp.h"

#define    NDG     2000    /* 要发送的 datagrams 的个数 */
#define    DGLEN   1400    /* 每个 datagram 的 length */

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
  int           i;
  char          sendline[MAXLINE];

  for (i = 0; i < NDG; i++) {
    Sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
  }
}
