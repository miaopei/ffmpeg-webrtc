#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define FD_SIZE 1024
#define MESSAGE_SIZE 1024

int main(){

  int ret = -1;

  int pid;
  int accept_fd = -1;
  int socket_fd = -1;
  int accept_fds[FD_SIZE] = {-1, };

  int curpos = -1;
  int maxpos = 0;
  int backlog = 10;
  int flags = 1; //open REUSEADDR option

  int max_fd = -1;
  fd_set fd_sets;
  int events=0;

  struct sockaddr_in local_addr, remote_addr;

  //create a tcp socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if ( socket_fd == -1 ){
    perror("create socket error");
    exit(1);
  }

  //set option of socket
  ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
  if ( ret == -1 ){
    perror("setsockopt error");
  }

  //NONBLOCK
  flags = fcntl(socket_fd, F_GETFL, 0);
  fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);

  //set local address
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(PORT);
  local_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(local_addr.sin_zero), 8);

  //bind socket
  ret = bind(socket_fd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in));
  if(ret == -1 ) {
    perror("bind error");
    exit(1);
  }

  ret = listen(socket_fd, backlog);
  if ( ret == -1 ){
    perror("listen error");
    exit(1);
  }

  max_fd = socket_fd; //每次都重新设置 max_fd
  for(int i=0; i< FD_SIZE; i++){
    accept_fds[i] = -1; 
  }  

  for(;;) {

    FD_ZERO(&fd_sets); //清空sets
    FD_SET(socket_fd, &fd_sets); //将socket_fd 添加到sets

    for(int k=0; k < maxpos; k++){
      if(accept_fds[k] != -1){
        if(accept_fds[k] > max_fd){
          max_fd = accept_fds[k]; 
        }
        printf("fd:%d, k:%d, max_fd:%d\n", accept_fds[k], k, max_fd);
        FD_SET(accept_fds[k], &fd_sets); //继续向sets添加fd
      }
    }

    //遍历所有的fd
    events = select( max_fd + 1, &fd_sets, NULL, NULL, NULL );
    if(events < 0) {
      perror("select");
      break;
    }else if(events == 0){
      printf("select time out ......");
      continue;
    }else if( events ){

      printf("events:%d\n", events);

      if( FD_ISSET(socket_fd, &fd_sets)){ // 如果来的是新连接
        printf("listen event :1\n");

        int a = 0;
        for( ; a < FD_SIZE; a++){
          if(accept_fds[a] == -1){
            curpos = a;
            break;
          }
        }

        if(a == FD_SIZE){
          printf("the connection is full!\n");
          continue;
        }

        int addr_len = sizeof( struct sockaddr_in );
        accept_fd = accept(socket_fd, (struct sockaddr *)&remote_addr, &addr_len); //创建一个新连接的fd

        int flags = fcntl(accept_fd, F_GETFL, 0); //取出新连接的 fd 的相关选项
        fcntl(accept_fd, F_SETFL, flags|O_NONBLOCK); //设置为非阻塞

        accept_fds[curpos] = accept_fd;

        if(curpos+1 > maxpos){
          maxpos = curpos + 1; 
        }

        if(accept_fd > max_fd){
          max_fd = accept_fd; 
        }

        printf("new connection fd:%d, curpos = %d \n",accept_fd, curpos);
      }

      for(int j=0; j < maxpos; j++ ){
        if( (accept_fds[j] != -1) && FD_ISSET(accept_fds[j], &fd_sets)){ //有事件时
          printf("accept event :%d, accept_fd: %d\n",j, accept_fds[j]);
          char in_buf[MESSAGE_SIZE];
          memset(in_buf, 0, MESSAGE_SIZE);
          int ret = recv(accept_fds[j], &in_buf, MESSAGE_SIZE, 0);
          if(ret == 0){
            close(accept_fds[j]);
            accept_fds[j] = -1;
          } 

          printf( "receive message:%s\n", in_buf );
          send(accept_fds[j], (void*)in_buf, MESSAGE_SIZE, 0);
        }
      }
    }
  }

  printf("quit server...\n");
  close(socket_fd);

  return 0;
}

