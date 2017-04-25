#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char **argv) {
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

  for (;;) {
    pause();
  }
}
