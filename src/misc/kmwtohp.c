#include <stdio.h>
#include "newdx.h"

#define STX 2
#define EOT 4
#define ESC 27
#define FF  12
#define PAGELENGTH 3000
#define min(a,b)  ((a<b) ? a : b)

static FILE *fpo;
static int rasno;

void to_lpt1(char c)
{
   putc(c,stdout);
}

void shoot(char *ras, int n)
{
   while((ras[n-1]==0) && (n>10)) n--;
   fprintf(stdout,"\033*b%iW",n);
   /*  laser("\033*b300W");  */
   while (n--) to_lpt1(*(ras++));
   if( (rasno&0x000F) == 15)  fprintf(stderr,".");
}

void laser(char *s)
{
   while(*s)
   {
      to_lpt1(*s);
      s++;
   };
}

void toprt(FILE *fp)
{
   int c,flag=1;
   /*   laser("%! \n statusdict begin \n 1 200 setemulation \n end  \004"); */
   c = '=';
   do{
      fprintf(stderr,"$");
      if(c == '=') {
	 while((c=getc(fp))!='$'){
	    if(c==EOF) return;
            putc(c,stdout);
            if(c=='\n') putc('\r',stdout);
         }
	 fprintf(stderr,"Z");
	 while((c=getc(fp))!='Z')
	    if(c==EOF) return;
	 dx('$');
      }
      rasno = 0;
      laser("\033E\033&l0O\033*p1x1Y\033*t300R\033*r0A");

      while((c=getc(fp))!='='){
	 if(c==EOF) return;
	 if((rasno=dx(c&0x7F))>PAGELENGTH) goto end_img;
      }
   end_img:
      dx('=');
      fprintf(stderr,"=\n");
      laser("\033*rB\033E");
   }while(flag);
}

int main(int argc, char *argv[])
{
   int c;
   FILE *fp=NULL;
   const char *fname="stdin";

   if(argc==2){
      fname=argv[1];
      fp=fopen(fname, "r");
   }
   if(fp==NULL) {
      fp=stdin;
   }
   set_dx(300);
   fprintf(stderr, " decompressing %s \n", fname);
   toprt(fp);
   fclose(fp);
   fprintf(stderr,"the END !!!\n");
   /*  laser("\033\1770\004");  */
}
