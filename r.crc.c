#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
/* #include "rpnmacros.h" */
#include <string.h>

int calc_crc(char *buffer,int *l,int *seed,int stride);
main(int argc,char **argv)
{
   int nblu,fd,crc16=0,stride;
   char *buffer;
#if defined(NEC)
#define BUFFERSIZE 4096*1024
#else
#define BUFFERSIZE 3840*1024
#endif
   int n,nbufs=0;

   if (argc >= 2) {
     fd = open64(argv[1],O_RDONLY
#ifdef DIRECTIO
          +O_DIRECT
#endif
          ,0);
     if (fd < 0) {
       fprintf(stderr,"r.crc: %s: %s\n",argv[1],strerror(errno));
       fprintf(stdout,"-1 %s\n",argv[1]);
       exit(2);
       }
     stride = 8;
     if (argc > 2)
       n = sscanf(argv[2],"%i",&stride);
#if defined(__convex__)
     if (stride < 16) {
       fprintf(stderr,"usage: r.crc filename [byte_stride]\n");
       fprintf(stderr,"byte_stride must be specified and >= 16 for CFS\n");
       exit(1);
     }
#endif
#ifdef DIRECTIO
     buffer=memalign(64*1024,BUFFERSIZE);
#else
     buffer=malloc(BUFFERSIZE);
#endif
     if(buffer==NULL) {
        fprintf(stderr,"r.crc: Cannot allocate buffer \n");
        exit(1);
     }
     while ((nblu = read(fd,buffer,BUFFERSIZE)) > 0) {
       nbufs++;
       crc16 = calc_crc(buffer,&nblu,&crc16,stride);
       }
     fprintf(stdout,"%d %s\n",crc16,argv[1]);
     }
   else {
     fprintf(stderr,"usage: r.crc filename [byte_stride]\n");
     }
}

char * product_id_tag="$Id$";
