CC=cc32
F77=f7732
SHELL= /bin/sh
RMNLIB= $(ARMNLIB)/$(NEC)lib/librmn32stack_LR.a
RMNLIBETA= $(ARMNLIB)/$(NEC)lib/librmnbeta.a

GENDEFINE= BIT32

.DEFAULT:
	co $@

r.date: r.date.c
	$(CC)  $(NEC) -O $(CFLAGS) -c r.date.c
	$(F77) $(NEC) -o $@_$(host)$(NEC) r.date.o $(RMNLIB) $(EXTRALIB)
	rm -f *.o

all:
	r.remotemake $(RELS) pollux r.date
	r.remotemake $(RELS) zephyr r.date host=irix5
	r.remotemake $(RELS) newton r.date
	r.remotemake galois $(RELS) r.date F77=cc32 EXTRALIB="'-L/usr/local/pgi/linux86/lib -lpgftnrtl -lpgc'" 
	make NEC=SX4 r.date

clean:
	rm -f *.o r.date_*
