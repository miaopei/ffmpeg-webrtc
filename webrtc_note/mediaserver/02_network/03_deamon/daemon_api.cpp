#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  if(daemon(0,0) == -1)
    exit(EXIT_FAILURE);
  while(1)
  {
    sleep(60);
  }
  return 0;
}
