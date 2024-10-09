#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static int	constant = 0;
static int	nsteps;
static char	*format;
static int max,min,incr;

void usage(void)
{
 	fprintf(stderr, "usage: seq [-fformat] [-w] [first [incr]] last\n");
	exit(1);
}

void buildfmt(void)
{
	char *dp;
	int w, p, maxw, maxp;
	static char fmt[16];
	char buf[32];
	double val;

	format = "%g\n";
	if(!constant)
		return;
	maxw = 0;
	maxp = 0;
	for(val = min; val <= max; val += incr){
		sprintf(buf, "%g", val);
		if(strchr(buf, 'e')!=0)
			return;
		dp = strchr(buf,'.');
		w = dp==0? strlen(buf): dp-buf;
		p = dp==0? 0: strlen(strchr(buf,'.')+1);
		if(w>maxw)
			maxw = w;
		if(p>maxp)
			maxp = p;
	}
	if(maxp > 0)
		maxw += maxp+1;
	sprintf(fmt,"%%%d.%df\n", maxw, maxp);
	format = fmt;
}

int main(int argc, char **argv){
	int j, n;
	char buf[256], ffmt[4096];
	double val;

	max=0 ; min=0 ; incr=0;
	argc-- ; argv++;
	if(**argv == '-' ){
	  char *ptr = argv[0]+1;
	  if(*ptr == 'w' ) constant++;
          argc-- ; argv++;
/*
	case 'f':
		format = ARGF();
		if(format[strlen(format)-1] != '\n'){
			sprintf(ffmt, "%s\n", format);
			format = ffmt;
		}
		break;
	default:
		goto out;
*/
	}
    out:
	if(argc<1 || argc>3)
		usage();
	max = atoi(argv[argc-1]);
	if(argc > 1)
		min = atoi(argv[0]);
	if(argc > 2)
		incr = atoi(argv[1]);
	if(incr == 0){
		fprintf(stderr, "seq: zero increment\n");
		exit(1);
	}
	if(!format)
		buildfmt();
	if(incr > 0){
		for(val = min; val <= max; val += incr){
			n = sprintf(buf, format, val);
			if(constant)
				for(j=0; buf[j]==' '; j++)
					buf[j] ='0';
			write(1, buf, n);
		}
	}else{
		for(val = min; val >= max; val += incr){
			n = sprintf(buf, format, val);
			if(constant)
				for(j=0; buf[j]==' '; j++)
					buf[j] ='0';
			write(1, buf, n);
		}
	}
	exit(0);
}
