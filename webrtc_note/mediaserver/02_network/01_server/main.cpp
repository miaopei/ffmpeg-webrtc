/**
 * a server
 *
 * @author lichao
 * @date 2019-08-07
 * @copyleft GPL 2.0
 * g++ -std=c++11 -g -o server main.cpp server.cpp
 */

#include <iostream>
#include "server.h"

int main(int argc, char* argv[])
{
  
  avdance::Server* server = new avdance::Server();

  if(server){
    server->run();
  }

  return 0;
}
