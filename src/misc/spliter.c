#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXCAR 128*1024

main(argc,argv)
int argc;
char *argv[];

{
   void splitit();

   int nl;
   char ligne[MAXCAR];
   
   nl = 0;
   while (fgets(ligne,MAXCAR,stdin) != NULL) {
    if ((ligne[0] != ' ') && (ligne[0] != '\t') && (! isdigit(ligne[0]))) {
      fputs(ligne,stdout);
      }
    else {
      splitit(ligne);
      }
    nl++;
    }
   }

void splitit(ligne)
char ligne[MAXCAR];
{
  int nc;
  char *pc;

  pc = &ligne[0];
  if (*pc == '\t')
     nc = 7;
  else
     nc = 0;
  while ((*pc != NULL) && (*pc != '\n') && (nc < MAXCAR)) {
    nc++;
    if ((*pc == '~') && (*(pc+1) == '~')) {
      fputs("\n      ",stdout);
      pc++;
      nc = 6;
      }
    else {
      if (nc > 72) {
        fputs("\n     %",stdout);
        putc(*pc,stdout);
        nc = 7;
        }
      else {
        putc(*pc,stdout);
        }
      }
    pc++;
  }
  putc('\n',stdout);
  }
