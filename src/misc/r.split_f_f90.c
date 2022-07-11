#include <stdio.h>

static int chop_bang=0;
static int f90=0;

#define MAXCAR 128*1024
#define Out(c) fputc(c,stdout)

static void splitit(ligne)
char ligne[MAXCAR];
{
  int nc;
  char *pc;
  char *space;
  char *amper;
  char quote_char=' ';

  pc = &ligne[0];
  if (*pc == '\t')
     nc = 7;
  else
     nc = 0;
  while ((*pc != '\0') && (*pc != '\n') && (nc < MAXCAR)) {
    if (*pc == ' '){
      space=pc;
      while (*pc == ' ') { 
      pc++;
      nc++;
      }
      if (*pc == '\n') {
	*space = '\n';
	  }
    }
    else {
	pc++;
	nc++;
    }
  
  }
  pc = &ligne[0];
  if (*pc == '\t')
     nc = 7;
  else
     nc = 0;
  while ((*pc != '\0') && (*pc != '\n') && (nc < MAXCAR)) {
    if(chop_bang) {
      if ((*pc == '"') || (*pc == '\'')) {
        if(*pc == quote_char) quote_char=' ' ;
        else                  quote_char=*pc ;
      }
      if ((*pc == '!') && (quote_char==' ')) break;
    }
    nc++;
    if ((*pc == '~') && (*(pc+1) == '~')) {
      Out('\n');Out(' ');Out(' ');Out(' ');Out(' ');Out(' ');Out(' ');
      pc++;
      nc = 6;
      }
    else {
      if ((nc > 72) && (f90==0)) {
        Out('\n');Out(' ');Out(' ');Out(' ');Out(' ');Out(' ');Out('%');
        Out(*pc);
        nc = 7;
        }
      else if ((nc > 131) && (f90!=0)) {
	
        Out('&');Out('\n');Out(' ');Out(' ');Out(' ');Out(' ');Out(' ');Out('&');
	space=pc;
	while(*pc == ' '){
	  pc++;
	}
	if(*pc == '&'){
	  amper=pc;	  
	  pc++;
	  while(*pc == ' '){
	    pc++;
	  }
	  /*	  if(*pc == '\n'){
	    *amper='\n';
	      pc=space;
	      }
	  else{*/
	    pc=space;
	    /*}*/
	  
	}
	else {
	  pc=space;
	}    
        Out(*pc);
        nc = 7;
        }
      else {
        Out(*pc);
        }
      }
    pc++;
  }
  Out('\n');
fflush(stdout);
  }

static void
outchar(char c)
{
static void safe_write2(int desc, char *ptr, int len);
static  char ligne[MAXCAR];
static  int indx=0;
int nc=0;

ligne[indx++]=c;

if(c=='\n' || indx>=MAXCAR-2) {
  ligne[MAXCAR-1]='\n';
  while(ligne[nc]==' ' || ligne[nc]=='\t') nc++;
  if(ligne[nc]=='\n') { indx=0; return; }  /* get rid of blank lines */
  if ((ligne[0] != ' ') && (ligne[0] != '\t') && (! isdigit(ligne[0]))) {
    indx=0 ;
    while(ligne[indx]!='\n') { Out(ligne[indx]); indx++; }
    Out('\n');
    }
  else {
    splitit(ligne);
    }
  indx=0;
  }
}

int main(int argc,char **argv)
{
  int c;

  while ( --argc ) {
    argv++;
    if(strcmp(*argv,"-f90")==0) { f90=1 ; fprintf(stderr,"setting f90 mode\n"); }
    if(strcmp(*argv,"-chop_bang")==0) { chop_bang=1 ; fprintf(stderr,"setting chop_bang mode\n"); }
  }
  while((c=fgetc(stdin)) != EOF ) outchar(c);
}
