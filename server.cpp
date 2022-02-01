#include "gsys.h"
#include "Sock2.h"
#include "ServerLog.h"
#include <stdio.h>
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
#define DEBUG

ServerLog *slog = new ServerLog("");
extern int testcon(char *,Sock2 *);

int main(int argc,char *argv[])
{
    Sock2 s;
    Sock2 c;
    int port;
    int ret;
    int i;
    fd_set readset;
    char buf[4096];
    char tbuf[1024];
    char *d;
    uint32_t len;

    if (argc < 2) {
	    slog->info((char *)"Must specify port #\n");
	    return 0;
    }
    port=atoi(argv[1]);
    if (port <1024 || port > 49149) {
	slog->info ((char *)"Invalid port #\n");
	return 0;
    }
    s.open(port); // serve main port
    
    if (s.fd == INVALID_SOCKET) {
        sprintf(buf,"Unable to serve Port:%d\n",port);
        slog->error(buf);
        return 0;
    }

    sprintf(buf,"Serving Port:%d",port);
    slog->info(buf);
    #ifdef TGOSLINUX
    signal(SIGCHLD,SIG_IGN);
    #endif
    while (1) {
        FD_ZERO(&readset);
        FD_SET(s.fd,&readset);
        ret=select((int)(s.fd+1),&readset,NULL,NULL,NULL);
        if (ret == -1 && errno == EINTR) {
            continue;
        }
        if (ret == -1 || s.fd == INVALID_SOCKET) {
            slog->error((char *)"Error Ret from select\n");
            return 0;
        }
        c.open(s);
        if (c.fd == INVALID_SOCKET) {
            slog->error((char *)"open failed ??\n");
            return 0;
        }
    break;
    }
    s.close();
    c.DoHandShake();
    if (c.st == HTMLSOCK) {
        // Sock2 saved the request header in hsbuf 
        if (memcmp(c.hsbuf,"GET / HTTP/1.1\r\n",16) != 0) {
            slog->info("Only know 1 page to server up");
        }
        else {
        // you cannot do a put must use put data
            strcpy(buf,"<!DOCTYPE html><head></head><body><H1>Hello World</h1></body>");
            sprintf(tbuf,"HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %u\r\n\r\n",
                            (uint32_t)strlen(buf));
            c.put_data(tbuf,strlen(tbuf));
            c.put_data(buf,strlen(buf));
#ifdef TGOSLINUX
            sleep(3);
#else
            Sleep(3000);
#endif
        }
        c.close();
        return 0;
    }
    for (i=0;i!=100;++i) {
        memset(buf,'A',200);
        buf[199]='\n';
        buf[200]='\0';
        sprintf(buf,"Hello Iteration %d",i);
        buf[strlen(buf)]=' ';
        if (Sock2::READY != c.put(buf,strlen(buf)+1)) {
            printf("error sending data\n");
            return 0;
        }
        if(Sock2::READY != c.getwait(&d,&len,-1)) {
            printf("error waiting for got it\n");
            return 0;
        }
        if (len > sizeof(buf)) {
            printf("write res greater than %d\n",(int)sizeof(buf));
            return 0;
        }
        memcpy(buf,d,len);
        free(d);
        if (strcmp(buf,"got it") != 0){
            printf("No got it returned:%s\n",buf);
            return 0;
        }
    }
    strcpy(buf,"!!!!");
    if(Sock2::READY != c.put(buf,strlen(buf)+1)) {
        printf("Error sending !!!!\n");
        return 0;
    }
    c.close();
    return 0;
}