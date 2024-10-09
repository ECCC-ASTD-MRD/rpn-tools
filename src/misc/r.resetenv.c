#include <rmn/rpnmacros.h>
#include <stdlib.h>

extern char *strcat();
extern int f77name(resetenv)();

int main(int argc , char **argv)
{
char buffer[4096];
int i;

f77name(resetenv)();
buffer[0]='\0';

for (i=1 ; i< argc ; i++) { strcat(buffer,argv[i]);strcat(buffer," ");};

system(buffer);
}
