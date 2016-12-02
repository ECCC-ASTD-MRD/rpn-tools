/* r.ip1 program 
   Author: Vivian Lee, RPN, December 5, 2002

Revision:
V2.0 Lee V., Mar  7, 2005 - convert from function to program
V3.0 Lee V., Jan 22, 2007 - adapt to take in negative values 
V4.0 Lee V., Oct 22, 2008 - an ip1code with no "-k" returned level_S and lev 
             but now returns level_S unless kind=3, then it will add "others"
V5.1 M. Lepine Mar 2014 - reload with librmn_014
V5.2 M. Lepine Dec 2014 - reload with librmn_015.1
V5.3 M. Lepine Fev. 2015 - reload with librmn_015.2
V5.4 M. Valin Dec. 2016 - correction au format d'impression pour l'option -k

*/
#include <rpnmacros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print_usage()
    {
      printf(" Usage : r.ip1 [-nk] ip1code\n");
      printf(" Result: value [level_type or kind]\n");
      printf(" Usage : r.ip1 [-no] [--]value kind\n");
      printf(" Result: ip1code(newstyle or oldstyle)\n");
      printf(" Formats :\n");
      printf(" options : -n to add end of line char\n");
      printf("         : -k to get code for kind \n");
      printf("         : -o to get ip1code in oldstyle \n");
      printf("         : -- to indicate value is negative \n");
      printf(" kind    : level_type\n");
      printf(" 0       : m  [metres] (height with respect to sea level)\n");
      printf(" 1       : sg [sigma] (0.0->1.0)\n");
      printf(" 2       : mb [mbars] (pressure in millibars)\n");
      printf(" 3       :    [others] (arbitrary code)\n");
      printf(" 4       : M  [metres] (height with respect to ground level)\n");
      printf(" 5       : hy [hybrid] (0.0->1.0)\n");
      printf(" 6       : th [theta]\n");
      printf("Example(1): r.ip1 -nk 1000\n");
      printf("1000.000000 2\n");
      printf("Example(2): r.ip1 -o 1000.0 2\n");
      printf("1000\n");
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
      f77name(convip)(&ip1,&lev,&kind,&mode,level_s,&flag,(F2Cl) 15);
      level_s[15] = '\0';
      if (kind_in_int) {
	  level_s[12] = '\0'; /* eliminer les caracteres pour le kind */
          printf("%#s %d%s",level_s,kind,cr);
        /*  printf("%f %d %s",lev,kind,cr); */
          }
      else if (kind == 3) {
              printf("%f others%s",lev,cr);
               }
      else printf("%#s%s",level_s,cr);
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
      f77name(convip)(&ip1,&lev,&kind,&mode,level_s,&flag,(F2Cl) 15);
      printf("%d %s",ip1,cr);
  } 
}

char * product_id_tag="$Id$";
