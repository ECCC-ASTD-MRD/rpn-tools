#include <unistd.h>
#include <sys/param.h>
#include <stdio.h>

int main(int argc , char **argv){
  char resolved[MAXPATHLEN+1];
  int nc = readlink(argv[1],resolved,MAXPATHLEN);
  resolved[nc]='\0';
  printf("%s",nc>0?resolved:"");
  return(0);
}
