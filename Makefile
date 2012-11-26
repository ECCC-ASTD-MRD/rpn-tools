FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include

OBJETS = r.crc.o

VER = 2.1

default: absolu

.ftn.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

absolu: $(OBJETS)
	s.compile -o r.crc_$(VER)-$(BASE_ARCH) -conly -obj $(OBJETS) -abi $(ABI) -librmn rmn_013

clean:
	rm r.crc_$(VER)-$(BASE_ARCH) $(OBJETS)

