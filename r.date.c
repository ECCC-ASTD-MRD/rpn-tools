/*Auteur: Simon Pellerin */
/* 9 Decembre 1996 */
/*Revision 1.01 Michel Valin */
/* 26 fevrier 1998 */
#include <rpnmacros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print_usage()
    {
      printf(" Usage   : r.date [-nVSLMM] date1 \n");
      printf(" Resultat: CMC date time stamp\n\n");
      printf(" Usage   : r.date [-nVSL] date1 +nhours\n");
      printf(" Resultat: YYYYMMDDHHMMSSHH=date1+nhours\n\n");
      printf(" Usage   : r.date [-nVSL] date1 -nhours\n");
      printf(" Resultat: YYYYMMDDHHMMSSHH=date1-nhours\n\n");
      printf(" Usage   : r.date [-nVSL] date1 date2\n");
      printf(" Resultat: nhours=date1-date2\n\n");
      printf(" Formats :\n");
      printf(" date1 et date2 (integer): YYYYMMDD[HH][MM][SS][HH] or 9 digit CMC timestamp\n");
      printf(" nhours  : real*8\n");
      printf(" options : -n mettre une fin de ligne, -L sortie en format eclate YYYY MM DD HH MM SS\n");
      printf("           -S sorti en format CMC timestamp, -V sortie en format YYYYMMDDHHMMSS00\n");
      printf("           -M multiplier le resultat par 60 -MM multiplier le resultat par 3600  \n");
      printf(" nhours  : nnn (heures), nnnS (secondes), nnnM (minutes), nnnD (jours), nnnW (semaines)\n");
      exit(1) ;
    }

int main(argc,argv)
int argc;
char *argv[];
{
  /* Declarations */
  int i;
  int factor_out = 1;
  long iout;
  int stamp0;
  int stamp1;
  int yyyymmdd;
  int lng_stmp=0;
  char YYYYMMDD[9];
  char HHMMSSHH[9];
  char *cr="\0";
  int hhmmsshh;
  char signe;
  int mode;
  double nhours,factor=1.0;
  int force_stamp=0;
  int force_visual=0;
  int force_long=0;
  char *options;
  char time_opt;
  int not_alphnum=1;

  while (argc > 1 && *argv[1]=='-') {
     options = argv[1];
     while (*++options){
       switch(*options){
       case 'V' : force_visual = 1; break ;
       case 'S' : force_stamp = 1; break ;
       case 'L' : force_long = 1; break ;
       case 'M' : factor_out *= 60 ; break ;
       case 'n' : cr="\n" ; break ;
       default  : print_usage(); break ;
         }
       }
      ++argv;
      --argc;
     }

  /* Usage : */
  if (argc < 2 ) print_usage();
  stamp0 = -1;
  if ( ((*argv[1] >= 'A' ) && (*argv[1] <= 'Z' )) || ((*argv[1] >= 'a' ) && (*argv[1] <= 'z' )) ) {
     stamp0 = f77name(iopdatm)(argv[1],strlen(argv[1]));
     lng_stmp = 9;
     }
  else if ( (lng_stmp=strlen(argv[1])) < 8 ) print_usage();
  /* Initialisation de la premiere date */
  if(*argv[1] == '=') {
     argv[1]++ ;
     lng_stmp = 9;
     }
  if ( lng_stmp != 9 ) {
    strncpy(YYYYMMDD,argv[1],8);
    YYYYMMDD[8]='\0';
    yyyymmdd=atoi(YYYYMMDD);
  
    strncpy(HHMMSSHH,&argv[1][8],strlen(argv[1])-8);
    for (i=strlen(argv[1])-8;i < 8; i++)
      HHMMSSHH[i]='0';

    HHMMSSHH[8]='\0';
    hhmmsshh=atoi(HHMMSSHH);

    /* construction de yyyymmdd a stamp0*/
    mode=3;
    f77name(newdate)(&stamp0,&yyyymmdd,&hhmmsshh,&mode);
    }
  else {
    if(stamp0==-1)stamp0=atoi(argv[1]);
    }
  if (argc==2) {
    if (  (lng_stmp != 9 || force_stamp) && !force_long && !force_visual) {   /* input was in visual format, print stamp */
      printf("%9.9d%s",stamp0,cr);
      }
    else {   /* input was in stamp format, print visual */
      mode = -3;
      f77name(newdate)(&stamp0,&yyyymmdd,&hhmmsshh,&mode);
      if ( force_long ) {
        printf("%4.4d %2.2d %2.2d %2.2d %2.2d %2.2d%s",yyyymmdd/10000,
               (yyyymmdd/100)%100,yyyymmdd%100,hhmmsshh/1000000,
               (hhmmsshh/10000)%100,(hhmmsshh/100)%100,cr);
        }
      else {
        printf("%8.8d%8.8d%s",yyyymmdd,hhmmsshh,cr);
        }
      }
    exit(0);
    }

  not_alphnum=1;
  if ( (*argv[2] >='A' && *argv[2] <= 'Z') ) not_alphnum=0;
  if ( (*argv[2] >='a' && *argv[2] <= 'z') ) not_alphnum=0;

  if ( strlen(argv[2]) < 8 && *argv[2] != '+' && *argv[2] != '-' 
      && *argv[2] != '='
      &&  not_alphnum ) print_usage();
  if (*argv[2] == '+' || *argv[2] == '-')
    {
      if(*argv[2] == '+') argv[2]++;
      nhours=atof(argv[2]);
      time_opt = argv[2][strlen(argv[2])-1] ;
      switch (time_opt) {
        case 'M': factor = factor / 60.0 ; break ;
        case 'S': factor = factor / 3600.0 ; break ;
        case 'D': factor = factor * 24.0 ; break ;
        case 'W': factor = factor * 7.0 * 24.0 ; break ;
        }
      nhours *= factor ;
      /* addition de nhours a stamp0 */
      f77name(incdatr)(&stamp1,&stamp0,&nhours);

      /* conversion de stamp1 a yyyymmdd hhmmsshh*/
      mode=-3;
      f77name(newdate)(&stamp1,&yyyymmdd,&hhmmsshh,&mode);

      if ( force_stamp) {
        printf("%9.9d%s",stamp1,cr);
        }
      else if ( force_long ) {
        printf("%4.4d %2.2d %2.2d %2.2d %2.2d %2.2d%s",yyyymmdd/10000,
               (yyyymmdd/100)%100,yyyymmdd%100,hhmmsshh/1000000,
               (hhmmsshh/10000)%100,(hhmmsshh/100)%100,cr);
        }
      else {
        printf("%8.8d%8.8d%s",yyyymmdd,hhmmsshh,cr);
        }
    }
  else
    {
      stamp1 = -1;
      if ( ((*argv[2] >= 'A' ) && (*argv[2] <= 'Z' )) ||
           ((*argv[2] >= 'a' ) && (*argv[2] <= 'z' ))     ) {
         stamp1 = f77name(iopdatm)(argv[2],strlen(argv[2]));
         lng_stmp = 9;
         }
      else lng_stmp=strlen(argv[2]);
      /* Initialisation de la deuxieme date */
      if(*argv[2] == '=') {
         argv[2]++ ;
         lng_stmp = 9;
         }
      if (lng_stmp != 9) {
        strncpy(YYYYMMDD,argv[2],8);
        YYYYMMDD[8]='\0';
        yyyymmdd=atoi(YYYYMMDD);
    
        strncpy(HHMMSSHH,&argv[2][8],strlen(argv[2])-8);
        for (i=strlen(argv[2])-8;i < 8; i++)
          HHMMSSHH[i]='0';

        HHMMSSHH[8]='\0';
        hhmmsshh=atoi(HHMMSSHH);

        /* construction de yyyymmdd hhmmsshh a stamp1*/
        mode=3;
        f77name(newdate)(&stamp1,&yyyymmdd,&hhmmsshh,&mode);
        }
      else {
        if(stamp1==-1) stamp1 = atoi(argv[2]);
        }
      /* calcul de la difference */
      f77name(difdatr)(&stamp0,&stamp1,&nhours);
      if ( factor_out != 1 ) {
        iout = nhours * factor_out + 0.5 ;
        printf("%d%s",iout,cr);
        }
      else {
        printf("%g%s",nhours,cr);
        }
    }
  return 0;
}
