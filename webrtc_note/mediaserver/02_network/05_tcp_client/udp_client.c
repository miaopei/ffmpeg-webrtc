#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char * *argv)
{   
    struct sockaddr_in addr;
    int sock;

    addr.sin_family     = AF_INET;
    addr.sin_port       = htons(9876);
    addr.sin_addr.s_addr = inet_addr("111.231.68.13");

    char buff_send[512] = "Hello";
    char buff_recv[512] = {0};
    int len = sizeof(addr);

    int n = 0;

    printf("This is a UDP client\n");

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      printf("socket error.\n");
      exit(1);
    }

    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
      printf("Incorrect ip address!");
      close(sock);
      exit(1);
    }

    n = sendto(sock, buff_send, strlen(buff_send), 0, (struct sockaddr *) &addr, sizeof(addr));
    if (n < 0)
    {
      printf("sendto error.\n");
      close(sock);
    }

    n = recvfrom(sock, buff_recv, 512, 0, (struct sockaddr *) &addr, &len);
    if (n > 0)
    {
        buff_recv[n] = 0;
        printf("received from sever:");
        puts(buff_recv);
    }
    else if (n == 0)
        printf("server closed.\n");
    else if (n == -1)
        printf("recvfrom error.\n");

    close(sock);
    
    return 0;
}

