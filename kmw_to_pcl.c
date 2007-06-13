#include <stdio.h>

#define STX 2
#define EOT 4
#define ESC 27
#define FF  12
#define PAGELENGTH 3000
#define min(a,b)  ((a<b) ? a : b)

static FILE *fpo;
static int rasno;

main(argc,argv)
int argc;
char *argv[];
{
  int c;
  FILE *fp,*fopen();

  fp=NULL;
  if(argc==7) fp=fopen(argv[6],"r");
  if(fp==NULL) {
    fp = stdin;
    }
  set_dx(300);
  laser("\033%-12345X@PJL\n");
  laser("@PJL SET RESOLUTION = 300\n");
  laser("\033%-12345X@PJL enter language=PCL\n");
  laser("ARG0=");laser(argv[0]);laser("\r\n");
  laser("ARG1=");laser(argv[1]);laser("\r\n");
  laser("ARG2=");laser(argv[2]);laser("\r\n");
  laser("ARG3=");laser(argv[3]);laser("\r\n");
  laser("ARG4=");laser(argv[4]);laser("\r\n");
  laser("ARG5=");laser(argv[5]);laser("\r\n");
/*
  toprt(fp);
*/
  fclose(fp);
  laser("\033E");
  laser("\033%-12345X@PJL RESET\n");
  laser("\033%-12345X");
  
}

toprt(fp)
FILE *fp;
{
   int c,flag=1;
/*   laser("%! \n statusdict begin \n 1 200 setemulation \n end  \004"); */
   c = '=';
   do{
      if(c == '=') {
	 while((c=getc(fp))!='$'){
	    if(c==EOF) return(0);
            putc(c,stdout);
            if(c=='\n') putc('\r',stdout);
            }
	 while((c=getc(fp))!='Z')
	    if(c==EOF) return(0);
	 dx('$');
	 }
      rasno = 0;
      laser("\033E\033&l0O\033*p1x1Y\033*t300R\033*r0A");

      while((c=getc(fp))!='='){
	 if(c==EOF) return(0);
	 if((rasno=dx(c&0x7F))>PAGELENGTH) goto end_img;
	 }
end_img:
      dx('=');
      laser("\033*rB\033E");
   }while(flag);
}

shoot(ras,n)
char *ras;
int n;
{
  while((ras[n-1]==0) && (n>10)) n--;
  fprintf(stdout,"\033*b%iW",n);
/*  laser("\033*b300W");  */
  while (n--) to_lpt1(*(ras++));
}

laser(s)
char *s;
{
while(*s){
  to_lpt1(*s);
  s++;
  };
}
to_lpt1(c)
char c;
{
   putc(c,stdout);
}
