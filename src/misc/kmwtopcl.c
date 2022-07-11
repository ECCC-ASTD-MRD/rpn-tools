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

  if(argc==2) fp=fopen(argv[1],"r");
  if(fp==NULL) {
    argv[1] = "stdin";
    fp = stdin;
    }
  set_dx(300);
  fprintf(stderr," decompressing %s \n",argv[1]);
  toprt(fp);
  fclose(fp);
  fprintf(stderr,"the END !!!\n");
  laser("\033\1770\004");
}

toprt(fp)
FILE *fp;
{
   int c,flag=1;
   laser("%! \n statusdict begin \n 1 200 setemulation \n end  \004");
   c = '=';
   do{
      fprintf(stderr,"$");
      if(c == '=') {
	 while((c=getc(fp))!='$'){
	    if(c==EOF) return(0);
            putc(c,stdout);
            if(c=='\n') putc('\r',stdout);
            }
	 fprintf(stderr,"Z");
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
      fprintf(stderr,"=\n");
      laser("\033*rB\014\033E");
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
  if( (rasno&0x000F) == 15)  fprintf(stderr,".");
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
