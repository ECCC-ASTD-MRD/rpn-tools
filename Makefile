.SUFFIXES :

.SUFFIXES : .ftn .o

SHELL = /bin/sh

CPP = /lib/cpp

FFLAGS =

CFLAGS =

OPTIMIZ = -O 2
$(info OPTIMIZ is ${OPTIMIZ})

CPPFLAGS = 

VER = 4.1

LIBRMN = 

default: absolu

.ftn.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

OBJET= ip1newstyle.o

FICHIERS = $(FDECKS)

absolu: $(OBJET)
	s.compile -o ip1newstyle_$(VER)-$(BASE_ARCH) -obj $(OBJET) -abi $(ABI) -librmn $(LIBRMN)

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
	rm *.o ip1newstyle_$(VER)-$(BASE_ARCH)
