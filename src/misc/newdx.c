#define min(a,b)  ((a<b) ? a : b)
#define MAXRAST 1024
static int RASTSIZE=300;
static char raster[MAXRAST];
static char xor;                         /* XOR status */
static short pat,oldpat;
static char *pnc, *limit;
static int rasno;
#include "newdx.h"

void shoot(char *ras, int n);

void dx_setmem(char *ras, int n, char c)
{
  while(n--) *(ras++) = c ;
}

void set_dx(int rsz)
{
  RASTSIZE = min(rsz,MAXRAST);
  rasno = 0;
  xor = 0;
  pnc = raster;
  pat = 128;     /* working word, holds 2 patterns at once */
  oldpat = 0;
  limit = pnc + RASTSIZE;
  dx_setmem(raster,RASTSIZE,'\0');
}

int dx(int cg)
{
    static  int i;
    static  unsigned char c;

    c=cg & 0x7F;                   /* start decoding      */
    if(c == '$') {
       rasno = 0;
       xor = 0;
       pnc = raster;
       pat = 128;     /* working word, holds 2 patterns at once */
       oldpat = 0;
       limit = pnc + RASTSIZE;
       dx_setmem(raster,RASTSIZE,'\0');
       return(0);
       }
    else if(c == '=') {
       rasno = 0;
       pnc = raster;
       pat = 128;     /* working word, holds 2 patterns at once */
       oldpat = 0;
       limit = pnc + RASTSIZE;
       return(-1);
       }
    else if(c < '0')
       return(rasno);
    else if(c <= '9') {            /* if a number         */
       pat = (pat << 4) + (c-'0');
       if ( pat<0 ) {                           /* if pattern full */
          if(pnc <= limit) {
              *pnc = ((char) pat) ^ ( *pnc & xor );
              pnc++;
              }
          oldpat = pat;
          pat = 128;
          }
       }
    else if(c < 'A')
       return(rasno);
    else if(c <= 'F') {              /* if letter A-F */
       pat = (pat << 4) + (c-'7');
       if ( pat<0 ) {                /* if patterns full */
          if(pnc <= limit) {
              *pnc = ((char) pat) ^ ( *pnc & xor );
              pnc++;
              }
          oldpat = pat;
          pat = 128;
          }
       }
    else  if(c <= 'N') {
       i = min(c-'F',limit-pnc);
       while(i-- > 0) {
          *pnc = ((char) oldpat) ^ (*pnc & xor);
          pnc++;
          }
       }
    else if(c <= 'T') {
                                /*  repeat pattern y times; y depends on c  */
       i = min(((c-'N')*16),limit-pnc);
       while(i-- > 0) {
          *pnc = ((char) oldpat) ^ (*pnc & xor);
          pnc++;
          }
       }
    else if(c == 'X')
       xor = 0xFF;
    else if(c == 'Y')
       xor = 0;
    else if(c == 'Z') {              /*  if end of raster  */
       rasno++;
       i = limit - pnc;
       while( i-- >= 0)        /*  fill rest of raster */
          *pnc++ &=  xor;       /*  with 0 or pnc */
       pnc = raster;           /*  reset pointer */
       pat = 128;
       oldpat = 0;
       shoot(raster,RASTSIZE);
       }
    return(rasno);
} /*  end dx  */

