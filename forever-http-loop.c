#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORTNUM 80

int main(int argc, char *argv[]) {
  char *res = "HTTP/1.0 200 OK\n\rContent-Length: 6\n\r\n\rBye.\r\n";
  char buf[512];

  struct sockaddr_in dest;
  struct sockaddr_in serv;

  int sock, consock;
  socklen_t socksize = sizeof(struct sockaddr_in);

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

  memset(&serv, 0, sizeof(serv));           /* zero struct before using */
  serv.sin_family = AF_INET;                /* set protocol to tcp/ip */
  serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set address to any interface */
  serv.sin_port = htons(PORTNUM);           /* set server port number */

  sock = socket(AF_INET, SOCK_STREAM, 0);

  bind(sock, (struct sockaddr *)&serv, sizeof(struct sockaddr));

  listen(sock, 1);

  for (;;) {
    consock = accept(sock, (struct sockaddr *)&dest, &socksize);
    printf("Incoming connection from %s - responding.\n", inet_ntoa(dest.sin_addr));
    send(consock, res, strlen(res), 0);
    recv(consock, buf, sizeof(buf), 0);
    sleep(1);  /* allow socket to drain before signalling the socket is closed */
    shutdown(consock, SHUT_RDWR);
    close(consock);
  }

  close(sock);
}
