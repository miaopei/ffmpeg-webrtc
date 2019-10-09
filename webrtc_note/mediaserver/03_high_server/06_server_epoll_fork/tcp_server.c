#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8111

#define FD_SIZE 20
#define MAX_EVENTS 20
#define TIME_OUT 500

#define MESSAGE_SIZE 1024

#define NB_PROCESS 4


int main(){

  int ret = -1;

  int socket_fd = -1;
  int accept_fd = -1;

  int flags = 1;
  int backlog = 10;

  struct sockaddr_in local_addr,remote_addr;

  struct epoll_event ev, events[FD_SIZE];
  int epoll_fd = -1; 
  int event_number = 0;

  int pid;
  int status;
  int max_subprocess = NB_PROCESS;

  //creat a tcp socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if ( socket_fd  == -1 ){
    perror("create socket error");
    exit(1);
  }

  //set REUSERADDR
  ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&flags, sizeof(flags)); 
  if ( ret == -1 ){
    perror("setsockopt error");
  }

  //set NONBLOCK
  flags = fcntl(socket_fd, F_GETFL, 0);
  fcntl(socket_fd, F_SETFL, flags|O_NONBLOCK);

  //set address
  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(PORT);
  local_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(local_addr.sin_zero),8);

  //bind addr
  ret = bind(socket_fd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in));
  if( ret == -1 ) {
    perror("bind error");
    exit(1);
  }

  if (listen(socket_fd, backlog) == -1 ){
    perror("listen error");
    exit(1);
  }

  //fork some subprocess
  for(int a=0; a < max_subprocess; a++){
    if(pid !=0){
      pid = fork(); 
    }
  }

  //child process
  if(pid == 0) {

    printf("create an new child process...");
    //create epoll
    epoll_fd = epoll_create(256);//the size argument is ignored
    ev.data.fd=socket_fd;
    ev.events=EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev); //将socket_fd 添加到epoll中
    for(;;){
      //events 表示一共有多少事件被侦听
      //MAX_EVENTS 表示在events个事件中，本次调用最多能返回多少个被解发的事件
      //TIME_OUT 表示本次调用最多等多长时间
      //event_number 表示本次调用真正有多少事件被解发
      event_number = epoll_wait(epoll_fd, events, MAX_EVENTS, TIME_OUT);
      for(int i=0; i < event_number; i++){

        if(events[i].data.fd == socket_fd){ // 如果是侦听端口的事件

          printf("listen event... \n");

          int addr_len = sizeof( struct sockaddr_in );
          accept_fd = accept(socket_fd, (struct sockaddr *)&remote_addr, &addr_len);

          //将新创建的socket设置为 NONBLOCK 模式
          flags = fcntl(accept_fd, F_GETFL, 0);
          fcntl(accept_fd, F_SETFL, flags|O_NONBLOCK);

          ev.data.fd=accept_fd;
          ev.events=EPOLLIN | EPOLLET;
          epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &ev);

          printf("new accept fd:%d\n",accept_fd);

        } else if(events[i].events & EPOLLIN){

          //printf("accept event :%d\n",i);

          char in_buf[MESSAGE_SIZE];
          memset(in_buf, 0, MESSAGE_SIZE);

          //receive data
          ret = recv( events[i].data.fd, &in_buf, MESSAGE_SIZE, 0 );
          if(ret == MESSAGE_SIZE ){
            printf("maybe have data....");
          }

          if(ret <= 0){

            switch (errno){
              case EAGAIN:
                ret = recv(events[i].data.fd, &in_buf, MESSAGE_SIZE, 0);
                break;
              case EINTR:
                printf("recv EINTR... \n");
                break;
              default:
                printf("the client is closed, fd:%d\n", events[i].data.fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &ev); 
                close(events[i].data.fd);
                ;
            }
          }

          printf(">>>receive message:%s\n", in_buf);
          send(events[i].data.fd, &in_buf, ret, 0);
        }
      }
    }

  }else {// pid == 0
    //wait child process to quit 
    wait(&status);
  }

  return 0;
}

