#include "gsys.h"
#ifdef TGOSLINUX
#include <signal.h>
#include <sys/socket.h>        /* shutdown() */
#include <netdb.h>			/* getservent() */
#include <netinet/in.h>        /* struct sockaddr_in, INADDR_NONE */
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>        /* inet_addr() */
#include <sys/ioctl.h>        /* ioctl() */
#include <sys/times.h>              /* times() */
#include <sys/termios.h>        /* fstat() */
#include <sched.h>
#include <fcntl.h>        /* fcntl() */
#include <string.h>        /* bcopy() */
#include <errno.h>        /* errno */
#include <stdlib.h>        /* exit() */
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>        /* close(),sleep(),chdir(), select(),getopt() */
#include <sys/types.h>
#include <sys/stat.h>        /* fstat() */
#endif
#include "Sock2.h"
#include "ServerLog.h"
#include <stdio.h>
#define DEBUG

ServerLog *slog = new ServerLog("");
extern int testcon(char *,Sock2 *);

int main(int argc,char *argv[])
{
    Sock2 c;
    char buf[4096];
    char *d;
    uint32_t len;

    c.open("192.168.1.67",5001,LONGLEN);
    //c.open("127.0.0.1",5001,SHORTLEN);
    
    if (c.fd == INVALID_SOCKET) {
        slog->error((char *)"open failed ??\n");
        return 0;
    }
    while(1) {
        if (Sock2::READY != c.getwait(&d,&len,-1)) {
            printf("c.getwait error waiting for data\n");
            return 0;
        }
        if (len > sizeof(buf)) {
            printf("write res greater than %d\n",(int)sizeof(buf));
            return 0;
        }
        memcpy(buf,d,len);
        free(d);
        if (memcmp(buf,"!!!!",4) == 0) {
            break;
        }
        printf("%s",buf);
        strcpy(buf,"got it");
        if (Sock2::READY != c.put(buf,strlen(buf)+1)) {
            printf("Error sending got it\n");
            return 0;
        }
    }
    c.close();
    return 0;
}
