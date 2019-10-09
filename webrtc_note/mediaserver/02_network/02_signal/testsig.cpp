#include <iostream>

#include <unistd.h>
#include <signal.h>

void sighandle(int sig)
{
    std::cout << "sighup received :" 
              << sig
              << std::endl;
}
int main(int argc,char **argv)
{
       signal(SIGHUP, sighandle);
       signal(SIGINT, sighandle);
       signal(SIGQUIT, sighandle);
       pause();
       return 0;
}
