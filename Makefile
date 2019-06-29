FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = 

OBJETS = r.crc.o

VER = 2.4

LIBRMN = rmn

default: absolu

.ftn.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

absolu: $(OBJETS)
	s.compile -o r.crc_$(VER)-$(BASE_ARCH) -conly -obj $(OBJETS) $(OPTIMIZ) -librmn $(LIBRMN)

clean:
	rm r.crc_$(VER)-$(BASE_ARCH) $(OBJETS)

