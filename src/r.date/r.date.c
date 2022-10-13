#include <rmn/rpnmacros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void print_usage()
    {
      printf(" r.date Version 1.12\n");
      printf(" Usage   : r.date [-nVSLMM] date1 [+ntime][SMDW]\n");
      printf(" Resultat: YYYYMMDDHHMMSSHH or CMC date time stamp\n");
      printf(" Example: r.date 2013041700 \n");
      printf(" Example: r.date 2013041700 +24\n");
      printf(" Example: r.date =-1274137515 +24S\n");
      printf(" Example: r.date 385856000 +1.5D\n");
      printf(" Usage   : r.date [-nVSLpP] date1 date2\n");
      printf(" Resultat: nhours (date1-date2)\n");
      printf(" Example: r.date 2013041700  2013041600\n");
      printf(" Formats Allowed:\n");
      printf(" date1/date2: YYYYMMDD[HH][MM][SS][HH] or 9 digit CMC timestep\n");
      printf(" ntime units: none:hour,S:sec,M:min,D:day,W:week\n");
      printf(" options : \n");
      printf(" -n newline char\n");
      printf(" -L output format YYYY MM DD HH MM SS\n");
      printf(" -S output format CMC timestamp\n");
      printf(" -V output format YYYYMMDDHHMMSS00\n");
      printf(" -M  multiply result by 60    \n");
      printf(" -MM multiply result by 3600  \n");
      printf(" -p print hours mins secs with C format_string \n");
      printf(" -P print hours mins secs with format format HHHMMSS\n");
      exit(1) ;
    }

int main(argc,argv)
int argc;
char *argv[];
{
  /* Declarations */
  int i;
  int factor_out = 1;
  long iout,minutes,seconds;
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
  char *out_format=NULL;
  int out_format_flag=0;
  

  while (argc > 1 && *argv[1]=='-') {
     options = argv[1];
     while (*++options){
       switch(*options){
       case 'V' : force_visual = 1; break ;
       case 'S' : force_stamp = 1; break ;
       case 'L' : force_long = 1; break ;
       case 'M' : factor_out *= 60 ; break ;
       case 'n' : cr="\n" ; break ;
       case 'p' : out_format_flag=1 ; break ;
       case 'P' : out_format_flag=1 ; out_format="%03d%02d%02d" ; break ;
       default  : print_usage(); break ;
         }
       }
      ++argv;
      --argc;
     }
  if(out_format_flag && (out_format == NULL) ) {
    out_format=argv[1];
    ++argv;
    --argc;
    }
  if(out_format_flag) factor_out = 3600 ;

  /* Usage : */
  if (argc < 2 ) print_usage();

  stamp0 = -1;
  if ( (lng_stmp=strlen(argv[1])) < 8 ) print_usage();

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

  if ( strlen(argv[2]) < 8 && *argv[2] != '+' && *argv[2] != '-' 
      && *argv[2] != '=') print_usage();

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
      lng_stmp=strlen(argv[2]);

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
      if(out_format_flag) {
        iout = nhours ;
        minutes = (nhours - iout) * 60.0 ;
        seconds = (nhours - iout - minutes/60.0 ) * 3600.0 + .5 ;
        printf(out_format,iout,minutes,seconds);
        }
      else {
        nhours *= factor_out ;
        printf("%g%s",nhours,cr);
        }
    }
  return 0;
}


char * product_id_tag="$Id$";
