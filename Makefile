FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include

OBJETS = filetyp.o

VER = 006

LIBRMN = rmn_014

default: absolu

.ftn.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

absolu: $(OBJETS)
	s.compile -o r.filetype_$(VER)-$(BASE_ARCH) -obj $(OBJETS) -abi $(ABI) -librmn $(LIBRMN)

clean:
	rm r.filetype_$(VER)-$(BASE_ARCH) $(OBJETS)

