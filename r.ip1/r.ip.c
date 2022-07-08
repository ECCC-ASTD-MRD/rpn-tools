/* r.ip program 
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
v5.5 V. Lee   May 2018  - add documentation on other kinds
v5.5.1 D. Bouhemhem   Jan. 2021  - add function to ckeck kind is a number
*/
#include <rpnmacros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int check_kind(char str[])
{
    int i = 0;

    //if negative number skip sign
    if (str[0] == '-')
        i = 1;
    for (; str[i] != 0; i++)
    {
        //if (str[i] > '9' || str[i] < '0')
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}


void print_usage()
    {
      printf(" Usage : r.ip [-nk] ipcode\n");
      printf(" Result: value [level_type or kind]\n");
      printf(" Usage : r.ip [-no] [--]value kind\n");
      printf(" Result: ipcode(newstyle or oldstyle)\n");
      printf(" Formats :\n");
      printf(" options : -n to add end of line char\n");
      printf("         : -k to get code for kind \n");
      printf("         : -o to get ipcode in oldstyle \n");
      printf("         : -- to indicate value is negative \n");
      printf(" kind    : level_type\n");
      printf(" 0       : m  [metres] (height with respect to sea level)\n");
      printf(" 1       : sg [sigma] (0.0->1.0)\n");
      printf(" 2       : mb [mbars] (pressure in millibars)\n");
      printf(" 3       :    [others] (arbitrary code)\n");
      printf(" 4       : M  [metres] (height with respect to ground level)\n");
      printf(" 5       : hy [hybrid] (0.0->1.0)\n");
      printf(" 6       : th [theta]\n");
      printf("10       : H  [hours]\n");
      printf("15       :    [reserved, integer]\n");
      printf("17       :    [index X of conversion matrix]\n");
      printf("21       : mp [pressure in metres]\n");
      printf("Example(1): r.ip -nk 1000\n");
      printf("1000.000000 2\n");
      printf("Example(2): r.ip -o 1000.0 2\n");
      printf("1000\n");
      exit(1) ;
    }

r_ip (argc, argv)
int argc;
char *argv[];
{
  /* Declarations */
  int kind_in_int = 0;
  int oldstyle = 0;
  int negative = 0;
  int ip1, flag;
  int mode, i;
  int kind = 9999;
  char *cr = "\0";
  char *options;
  float lev;
  char level_s[16];

  
  while (argc > 1 && *argv[1] == '-')
  {
     options = argv[1];
     while (*++options)
     {
       switch(*options)
       {
	       case 'k' : kind_in_int = 1; break ;
	       case 'n' : cr = "\n" ; break; 
	       case 'o' : oldstyle = 1 ; break ;
	       case '-' : negative = 1; argv[1] = options; while(*++options); break;
	       default  : print_usage(); break ;
       }
     }
     
     if (negative) 
	      break;
      ++argv;
      --argc;
  }


  if ( (argc < 2) || (argc > 3) ) 
	  print_usage();

  if (argc == 2) 
  {
      mode = -1;
      flag = 1;
      sscanf(argv[1], "%d", &ip1);

      f77name(convip_plus)(&ip1, &lev, &kind, &mode, level_s, &flag, (F2Cl) 15);
      level_s[15] = '\0';
      if (kind_in_int) 
      {
          for(i = 0 ; i < 15 ; i++) 
		  if(level_s[i] != ' ') break;

	  /* get first non blank char */
          for(i = i; i < 15; i++)
	  { 
		  if(level_s[i] == ' ')
			  level_s[i] = '\0';
	  }
          /* replace blanks with nulls */
          printf("%#s %d %s", level_s, kind, cr);

      }
      else if (kind == 3) 
      {
  	      printf("%f others %s", lev, cr);
      }
      else printf("%#s %s", level_s, cr);
  } 


  if (argc == 3) 
  {
      mode = 2;
      int ret = 20;
      if (oldstyle) 
	      mode = 3;
      flag = 0;
      sscanf(argv[1], "%f", &lev);

      if(!check_kind(argv[2]))
      {      
        printf("Invalid kind = %s\n", argv[2]);
	exit(1);
      }

      ret = sscanf(argv[2],"%d", &kind);

      f77name(convip_plus)(&ip1, &lev, &kind, &mode, level_s, &flag, (F2Cl) 15);
      printf("%d %s",ip1, cr);
  } 
}

char * product_id_tag = "$Id$";
