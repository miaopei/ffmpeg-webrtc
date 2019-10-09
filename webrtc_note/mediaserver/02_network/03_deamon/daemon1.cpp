#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void daemonize(){
  int fd0;
  pid_t pid;

  /* * Become a session leader to lose controlling TTY. */
  if ((pid = fork()) < 0) {//注释2
    printf("can't fork \n");
    exit(-1);
  }
  else if (pid != 0) /* parent */
    exit(0);

  setsid();//注释3

  if (chdir("/") < 0) {//注释5
    printf("can't change directory to / \n");
    exit(-1);
  }

  fd0 = open("/dev/null", O_RDWR);//注释7
  dup2(fd0, STDIN_FILENO);
  dup2(fd0, STDOUT_FILENO);
  dup2(fd0, STDERR_FILENO);
}

int main(int argc, char* argv[])
{
  daemonize();

  while(1)
  {
    sleep(60);
  }

}
