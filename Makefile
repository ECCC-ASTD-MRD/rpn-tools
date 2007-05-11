.SUFFIXES :

.SUFFIXES : .c .o

SHELL = /bin/sh

CPP = /lib/cpp

FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include

default: absolu

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

OBJET= r.ip1.o

FICHIERS = $(FDECKS)

absolu: $(OBJET)
	r.build -o r.ip1 -obj $(OBJET) -bidon c -main r_ip1 -arch $(ARCH) -abi $(ABI) -librmn rmn_rc009

clean:
#Faire le grand menage. On enleve tous les fichiers sources\ninutiles et les .o 
	-if [ "*.ftn" != "`echo *.ftn`" ] ; \
	then \
	for i in *.ftn ; \
	do \
	fn=`r.basename $$i '.ftn'`; \
	rm -f $$fn.f; \
	done \
	fi
	rm *.o r.ip1
