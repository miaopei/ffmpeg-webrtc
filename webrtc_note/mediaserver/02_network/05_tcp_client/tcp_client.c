#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8111
#define MESSAGE_LENGTH 1024

int main()
{

  int ret = -1;
  int socket_fd;

  //server addr
  struct sockaddr_in serverAddr;

  char sendbuf[MESSAGE_LENGTH];
  char recvbuf[MESSAGE_LENGTH];

  int data_len;

  if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
    return 1;
  }

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);

  //inet_addr()函数，将点分十进制IP转换成网络字节序IP
  serverAddr.sin_addr.s_addr = inet_addr("39.105.185.198");
  //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(socket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
  {
    perror("connect");
    return 1;
  }

  printf("success to connect server...\n");

  while(1)
  {
    memset(sendbuf, 0, MESSAGE_LENGTH);

    printf("<<<<send message:");

    gets(sendbuf);
    //printf("\n");
    ret = send(socket_fd, sendbuf, strlen(sendbuf), 0);
    if(ret <= 0 ){
      printf("the connection is disconnection!\n"); 
      break;
    }

    if(strcmp(sendbuf, "quit") == 0){
      break;
    }

    printf(">>> echo message:");

    recvbuf[0] = '\0';
    data_len = recv(socket_fd, recvbuf, MESSAGE_LENGTH, 0);

    recvbuf[data_len] = '\0';

    printf("%s\n", recvbuf);

  }

  close(socket_fd);

  return 0;

}

