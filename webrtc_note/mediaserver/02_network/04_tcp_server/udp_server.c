#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char * *argv)
{
  struct sockaddr_in addr;
  addr.sin_family     = AF_INET;
  addr.sin_port       = htons(9876);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  char buff_recv[512] = {0};
  char buff_send[512] = "world";

  struct sockaddr_in clientAddr;
  int n;
  int len = sizeof(clientAddr);

  int sock;   

  printf("Welcome! This is a UDP server.\n");

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
      printf("socket error.\n");
      exit(1);
  }

  if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
  {
    printf("bind error.\n");
    exit(1);
  }

  while (1)
  {
    n = recvfrom(sock, buff_recv, 511, 0, (struct sockaddr *) &clientAddr, &len);
    if (n > 0)
    {
      buff_recv[n] = 0;
      printf("recv data from client:%s %u says: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff_recv);

      n = sendto(sock, buff_send, n, 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
      if (n < 0)
      {

        printf("sendto error.\n");
        break;

      }else {
        printf("recv error.\n");
        break;
      }
    }

    return 0;
  }

