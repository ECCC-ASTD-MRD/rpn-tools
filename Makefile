.SUFFIXES :

.SUFFIXES : .c .F .f .o .a .f90

FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = 

OBJETS = filetyp.o

VER = 012

LIBRMN = rmn

default: absolu

.ftn.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f90.o:
	s.f90 $(OPTIMIZ) -c $<

absolu: $(OBJETS)
	s.compile -o r.filetype_$(VER)-$(BASE_ARCH) -obj $(OBJETS) $(OPTIMIZ) -librmn $(LIBRMN)

clean:
	rm r.filetype_$(VER)-$(BASE_ARCH) $(OBJETS)

