#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int ier;
    struct stat buf;

    ier = stat(argv[1],&buf);

    if((int) buf.st_uid != (int) getuid())
          exit(1);
    else
          exit(0);
}
