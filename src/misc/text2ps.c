
/*
 *	text2ps
 *
 *	Convert plain text to postscript
 *	- Stephen Frede, UNSW, Australia
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	TRUE		1
#define	FALSE		0
#define	INCH		72.0		/* no. postscript units / inch */
#define	CM		28.35		/* postscript units / cm */
#define NEW_PAGE	014		/* ^L forces a new page */

#define	OPAGEOFFSET	(1.0*CM)	/* default hor offset if option used */
#define DTOPMARGIN	(0.5*CM)	/* default top margin */
#define OTOPMARGIN	(1.5*CM)	/* default top margin if option used */
#define	DFONTSIZE	10.0		/* default font size (in points) */
#define	OFONTSIZE	12.0		/* default font size if option used */
#define CHARSPERLINE	10000		/* no. of chars/line */
#define LINESPERPAGE	10000		/* no. of lines/page */
#define	ROTATION	0.0		/* default orientation */
#define	FONT		"Courier"
#define	TABSIZE		8


void init(
float fontsize,
float opageoffset,
float epageoffset,
float topmargin,
float linepitch,
float rotation,
char *fontname,
float spacing);
void process(FILE *istr);
void pch(int ch);
double mon_atof(char s[]);

/* typedef char	int; */

FILE		*ostr;

char *usage = "Valid text2ps options:\n\t-r[angle]\n\t-f[font]\n\t-s[size]\n\t-h[space]\n\t-p[pitch]\n\t-o[offset]\n\t-oe[offset]\n\t-m[topmargin]\n\t-w[width]\n\t-l[length]\n\t-v\n\t-?\n";
int	tabsize,charsperline,  /* tab spacing in # chars, # chars per line */
	linesperpage;          /* # lines per page */

int main(int argc, char	**argv)
{
	int	status = 0;	/* exit status (no. errors occured) */
	float	opageoffset,
		epageoffset,
		topmargin,
		fontsize,
		linepitch,
		spacing,
		rotation;
	char	*fontname;
	FILE	*istr;

	fontsize = DFONTSIZE;
	linepitch = 0.0;
	opageoffset = 0.0;
	epageoffset = 0.0;
	topmargin = DTOPMARGIN;
	spacing = 0.0;
	tabsize = TABSIZE;
	charsperline = CHARSPERLINE;
	linesperpage = LINESPERPAGE;
	rotation = ROTATION;
	fontname = FONT;
	ostr = stdout;
	argv++;		/* skip program name */
	while(*argv && **argv == '-')
	{
		char	c;

		(*argv)++;	/* skip the '-' */
		c = **argv;	/* option letter */
		(*argv)++;	/* skip option letter */
		switch(c)
		{
			case 'o':	/* offset */
				if (**argv != 'e') {
					if(**argv == '\0')
						opageoffset = OPAGEOFFSET;
					else
						opageoffset = mon_atof(*argv) * CM;
					epageoffset = opageoffset;
					}
				else {
					(*argv)++;
					if (**argv == '\0')
						epageoffset = OPAGEOFFSET;
					else 
						epageoffset = mon_atof(*argv) * CM;
					}
				break;

			case 'm':	/* top margin */
				if(**argv == '\0')
					topmargin = OTOPMARGIN;
				else
					topmargin = mon_atof(*argv) * CM;
				break;

			case 'r':	/* rotation */
				if(**argv == '\0')
					rotation = 90.0;
				else
					rotation = mon_atof(*argv);
				break;

			case 'p':	/* pitch (line spacing) */
				linepitch = mon_atof(*argv);
				break;

			case 's':	/* font size */
				if(**argv == '\0')
					fontsize = OFONTSIZE;
				else
					fontsize = mon_atof(*argv);
				break;

			case 't':	/* tab size */
				if(**argv == '\0')
					tabsize = 4;
				else
					tabsize = (int) mon_atof(*argv);
				break;

			case 'f':	/* font */
				if(**argv == '\0')
					fontname = "Times-Roman";
				else
					fontname = *argv;
				break;

			case 'h':	/* horizontal spacing */
				if(**argv == '\0')
					spacing = 0.25;
				else
					spacing = mon_atof(*argv);
				break;

			case 'w':	/* linewidth (chars per line) */
				if(**argv == '\0')
					charsperline = 72;
				else
					charsperline = (int) mon_atof(*argv);
				break;

			case 'l':	/* lines per page) */
				if(**argv == '\0')
					linesperpage = 60;
				else
					linesperpage = (int) mon_atof(*argv);
				break;

			case 'v':	/* version # */
				fprintf(stderr,"Version 1.04\n");
				exit(status);
				break;

			case '?':	/* usage - options */
				fprintf(stderr, "%s", usage);
				exit(status);
				break;

			default:
		                (*argv)--; (*argv)--;
				fprintf(stderr, "Illegal option: [%s]\n",
					*argv);
		                (*argv)++; (*argv)++;
				status++;
				break;
		}
		argv++;
	}
	if(status)
	{
           fprintf(stderr, "%s", usage);
		exit(status);
		/* NOTREACHED */
	}
	if(linepitch == 0)
		linepitch = fontsize + 2;
	spacing *= fontsize;
	init(fontsize, opageoffset, epageoffset, topmargin, linepitch, rotation, fontname, spacing);
	if(! *argv)
		process(stdin);
	else while(*argv)
	{
		if((istr = fopen(*argv, "r")) == NULL)
		{
			perror(*argv);
			status++;
		}
		else
		{
			process(istr);
			fclose(istr);
		}
		argv++;
	}
	putc('\004', ostr);
	exit(status);
}

void process(FILE *istr)
{
/*	register char	ch;  */
	register int    ch;
	register int	x,	/* used for tab calculations and folding */
		 	linenum;

        linenum = 1;
	x = 0;
	putc('(', ostr);
	while((ch=getc(istr)) != EOF)
	{
		if(ch < ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != NEW_PAGE)
		{
			ch = '?';
		}
		if(ch == '\t')
		{
			int	n = x + tabsize - (x % tabsize);

			while(x < n) {
					pch(' ');
					x++;
				}
		}
		else if(ch == '\n')
		{
			fprintf(ostr, ") n\n");
			linenum = linenum + 1;
			if (linenum > linesperpage) {
				fprintf(ostr,"p\n");
				linenum = 1;
			}
			x = 0;
			putc('(', ostr);
		}
		else if(ch == '\r')
		{
			fprintf(ostr, ") r\n");
			x = 0;
			putc('(', ostr);
		}
		else if(ch == NEW_PAGE)
		{
			fprintf(ostr, ") n p\n");
			linenum = 1;
			x = 0;
			putc('(', ostr);
		}
		else
		{
			if (x >= charsperline) {
				fprintf(ostr, ") n\n");
				linenum = linenum + 1;
				if (linenum > linesperpage) {
					fprintf(ostr,"p\n");
					linenum = 1;
				}
				x = 0;
				putc('(', ostr);
			}
			pch(ch);
			x++;
		}
	}
	fprintf(ostr, ") n p\n\f");
}

char	*inittab[] = {
        /* put out a minimal postscript header */
        "%!PS-Adobe-\n",
	/* print a page and start a new one */
	"/p",
	"{ 0.0 coffset sub 0 translate",
        "  /temp coffset def",
        "  /coffset noffset def",
	"  /noffset temp def",
	"  coffset 0 translate",
	"  copypage erasepage newpath 0 pgtop moveto",
	"} def",
	"/n",
	/* show the string given as an arg */
	"{ spacing 0 3 -1 roll ashow",
	/* now move down a line; linepitch is -'ve */
	"  0 linepitch rmoveto",
	/* save the new y posn */
	"  /y currentpoint exch pop def",
	/* test if the next line would be below the bottom margin */
	"  y 0 lt",
	/* if so, print the page, and move to the top of a new page */
	"  { p }",
	/* else go to where the next line is due to start */
	"  { 0 y moveto } ifelse",
	"} def",
	"/r",
	/* show the string given as an arg */
	"{ spacing 0 3 -1 roll ashow",
	/* save y */
	"  /y currentpoint exch pop def",
	/* and then move to the beginning of the current line */
	"  0 y moveto",
	"} def",
	(char *)0 };

void init(
float fontsize,
float opageoffset,
float epageoffset,
float topmargin,
float linepitch,
float rotation,
char *fontname,
float spacing)
{
	register char	**p;

	p = inittab;
	while(*p)
		fprintf(ostr, "%s\n", *p++);
	fprintf(ostr, "/%s findfont %.1f scalefont setfont\n",
		fontname, fontsize);
	fprintf(ostr, "/linepitch %.1f def\n", -linepitch);
	fprintf(ostr, "/spacing %.1f def\n", spacing);
	fprintf(ostr, "/coffset %.1f def\n", opageoffset + 4);
	fprintf(ostr, "/noffset %.1f def\n", epageoffset + 4);
	/* apply rotation transformation, if any */
	if(rotation != 0.0)
		fprintf(ostr, "%.1f rotate\n", rotation);
	/* get current imageable area */
	fprintf(ostr, "clippath pathbbox\n");
	/* save the upper right y coordinate */
	fprintf(ostr, "/pgtop exch def\n");
	/* save lower left y; translate origin to lower left */
	fprintf(ostr, "pop /y exch def y translate\n");
	/* subtract old lower left from upper right to get top of page */
	/* then subtract top margin to set top of page */
	fprintf(ostr, "/pgtop pgtop y sub %.1f sub linepitch add def\n", topmargin);
	/* apply horizontal offset, if any */
	/* unfortunately, a slight fudge factor is required here */
	fprintf(ostr, "coffset 0 translate\n");
	/* move to top of page, ready to start printing */
	fprintf(ostr, "newpath 0 pgtop moveto\n");
}

void pch(int ch)
{
	if(ch < ' ' || ch > '~')
		fprintf(ostr, "\\%3.3o", ch);
	else
	{
		if(ch == '(' || ch == ')' || ch == '\\')
			putc('\\', ostr);
		putc(ch, ostr);
	}
}

double mon_atof(char s[]) /* convert string to double */
{
     double val, power;
     int i;

     for (i=0; s[i]==' ' || s[i]=='\t'; i++) ;  /* skip white space or tabs */
     if (s[i] == '+' || s[i] == '-')            /* sign -if '-', ignore it */
         i++;
     for (val = 0; s[i] >= '0' && s[i] <= '9'; i++)
          val = 10 * val + s[i] - '0';
     if (s[i] == '.') {
         i++;
         for (power = 1; s[i] >= '0' && s[i] <= '9'; i++) {
              power = power /10;
              val =  power * (s[i] - '0') + val;
         }
     }
     return (val);
}


