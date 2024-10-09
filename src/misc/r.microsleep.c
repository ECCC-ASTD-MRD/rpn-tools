#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc , char **argv)
{
  unsigned long msec;
  unsigned long sec;

  if ( argc < 2 || argv[0] == 0 )
   {
     printf("Usager: %s msec \n", argv[0]);
     return(1);
   }

  msec=atoi(argv[1]);
  sec=msec/1000000;
  msec=msec-(sec*1000000);

#ifdef DEBUG
  printf("sleeping:%d: seconds\n",sec);
#endif
  if(sec > 0) sleep(sec);

#ifdef DEBUG
  printf("sleeping:%d: microseconds\n",msec);
#endif

  usleep(msec);
  return(0);
}
