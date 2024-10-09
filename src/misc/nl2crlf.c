#include <stdio.h>
int main()
{
int cc;
while((cc=getc(stdin))!=EOF){
   if(cc=='\n') putc('\r',stdout);
   putc(cc,stdout);
}
/* putc('\014',stdout); */
}
