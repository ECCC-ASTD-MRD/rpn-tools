FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = 

OBJETS = filetyp.o

VER = 008

LIBRMN = rmn_015.1

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

