#include <arpa/inet.h>          /* inet_ntoa */
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LISTENQ  1024  /* second argument to listen() */
#define PORTNUM 80

/* Simplifies calls to bind(), connect(), and accept() */
typedef struct sockaddr SA;

int open_listenfd(int port){
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;

    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval , sizeof(int)) < 0)
        return -1;

    // 6 is TCP's protocol number
    // enable this, much faster : 4000 req/s -> 17000 req/s
    if (setsockopt(listenfd, 6, TCP_CORK,
                   (const void *)&optval , sizeof(int)) < 0)
        return -1;

    /* Listenfd will be an endpoint for all requests to port
       on any IP address for this host */
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
        return -1;
    return listenfd;
}

void process(int fd, struct sockaddr_in *clientaddr){
    char *res = "HTTP/1.1 200 OK\r\nContent-Length: 6\r\nConnection: close\r\n\r\nBye.\r\n";
    char buf[256];

    printf("accept request, fd is %d, pid is %d, ip is %s.\n", fd, getpid(), inet_ntoa(clientaddr->sin_addr));

    recv(fd, buf, sizeof(buf), 0);
    send(fd, res, strlen(res), 0);
}

int main(int argc, char** argv){
    struct sockaddr_in clientaddr;
    int default_port = PORTNUM,
        listenfd,
        connfd;
    socklen_t clientlen = sizeof clientaddr;

    setbuf(stdout, NULL); // do not buffer stdout

    /* handle signals being PID 1 */
    signal(SIGHUP, exit);
    signal(SIGINT, exit);
    signal(SIGQUIT, exit);
    signal(SIGTRAP, exit);
    signal(SIGABRT, exit);
    signal(SIGKILL, exit);
    signal(SIGUSR1, exit);
    signal(SIGUSR2, exit);
    signal(SIGTERM, exit);
    signal(SIGSTOP, exit);

    // Ignore SIGPIPE signal, so if browser cancels the request, it
    // won't kill the whole process.
    signal(SIGPIPE, SIG_IGN);

    listenfd = open_listenfd(default_port);
    if (listenfd > 0) {
        printf("listen on port %d, fd is %d.\n", default_port, listenfd);
    } else {
        perror("ERROR");
        exit(listenfd);
    }

    for(int i = 0; i < 10; i++) {
        int pid = fork();
        if (pid == 0) {         //  child
            while(1){
                connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
                process(connfd, &clientaddr);
                close(connfd);
            }
        } else if (pid > 0) {   //  parent
            printf("child pid is %d.\n", pid);
        } else {
            perror("fork");
        }
    }

    while(1){
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        process(connfd, &clientaddr);
        close(connfd);
    }

    return 0;
}
