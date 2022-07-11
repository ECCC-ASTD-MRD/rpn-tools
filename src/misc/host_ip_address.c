#include <stdio.h>
#include <netdb.h>
/*
  one argument, the host name for which the IPv4 address is needed

  on stderr, error messages
  on stdout, the IPv4 address   a.b.c.d


  example : 
      host_ip_address www.microsoft.com
  has been tested on Linux, IRIX, AIX5
*/
main(int argc , char **argv)
{
     int **addr_list;
     struct hostent *answer;
     int ipaddr = 0;
     int b0, b1, b2, b3;
     char *hostname=argv[1];

     if ( argc != 2 )
       {
         fprintf(stderr, "Usage: host_ip_address desired_host_name\n");
         printf("127.0.0.1\n");
         exit(1);
       }
     if(NULL == (answer = gethostbyname(hostname)))
       {
         fprintf(stderr, "Cannot get address for host=%s\n", hostname);
         printf("127.0.0.1\n");
         exit(1);
       }

     addr_list = (int **)answer->h_addr_list;
     ipaddr = ntohl(**addr_list);

     b0 = ipaddr >> 24; b1 = ipaddr >> 16 ; b2 = ipaddr >> 8 ; b3 = ipaddr;
     b0 &= 255;
     b1 &= 255;
     b2 &= 255;
     b3 &= 255;

     printf("%d.%d.%d.%d\n", b0, b1, b2, b3);
     return(0);

}
