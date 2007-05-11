/* r.ip1 program 
   Author: Vivian Lee, RPN, December 5, 2002 
*/
#include <rpnmacros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print_usage()
    {
      printf(" Usage : r.ip1 [-nk] ip1code \n");
      printf(" Result: value kind\n\n");
      printf(" Usage : r.ip1 [-no] value kind\n");
      printf(" Usage : r.ip1 [-no] --value kind (for neg values)\n");
      printf(" Result: ip1code\n\n");
      printf(" Formats :\n");
      printf(" options : -n to add eol \n");
      printf("         : -k to get code for kind \n");
      printf("         : -o to get ip1code in oldstyle \n\n");
      printf(" kind    : level type\n");
      printf(" 0       : height [m] (metres)\n");
      printf(" 1       : sigma  [sg] (0.0->1.0)\n");
      printf(" 2       : pressure [mb] (millibars)\n");
      printf(" 3       : arbitrary code\n");
      printf(" 4       : height [M] (metres) with respect to ground level\n");
      printf(" 5       : hybrid coordinates [hy] (0.0->1.0)\n");
      printf(" 6       : theta [th]\n");
      exit(1) ;
    }

r_ip1 (argc,argv)
int argc;
char *argv[];
{
  /* Declarations */
  int kind_in_int=0;
  int oldstyle=0;
  int negative=0;
  int ip1,flag;
  int mode,kind;
  char *cr="\0";
  char *options;
  float lev;
  char level_s[16];

  
  while (argc > 1 && *argv[1]=='-') {
     options = argv[1];
     while (*++options){
/*       printf("*options=%s \n",options); */
       switch(*options){
       case 'k' : kind_in_int  = 1; break ;
       case 'n' : cr="\n" ; break ;
       case 'o' : oldstyle = 1 ; break ;
       case '-' : negative = 1; argv[1]=options; while(*++options); break;
       default  : print_usage(); break ;
         }
       }
      if (negative) break;
      ++argv;
      --argc;
      }

/*  printf("argc=%d\n",&argc); */

  if ( (argc < 2) || (argc > 3) ) print_usage();
  if (argc == 2) {
      mode=-1;
      flag=1;
      sscanf(argv[1],"%d",&ip1);
/*      ip1 = atoi(argv[1]);
        printf("ip1=%d\n",ip1); printf("calling convip\n"); */
      f77name(convip)(&ip1,&lev,&kind,&mode,level_s,&flag,15);
      level_s[15] = '\0';
      if (kind_in_int) {
          printf("%f %d %s",lev,kind,cr);
          }
      else printf("%#15s %f %s",level_s,lev,cr);
  } 

/*     printf("argc=%d\n",argc); */

  if (argc == 3) {
      mode=2;
      if (oldstyle) mode=3;
      flag=0;
      sscanf(argv[1],"%f",&lev);
      sscanf(argv[2],"%d",&kind);
/*      lev = atof(argv[1]);
        kind = atoi(argv[2]); 
        printf("argv=%s\n",argv[1]); 
        printf("scanned %f\n",lev); 
        printf("scanned %d\n",kind); */
      f77name(convip)(&ip1,&lev,&kind,&mode,level_s,&flag,15);
      printf("%d %s",ip1,cr);
  } 
}
