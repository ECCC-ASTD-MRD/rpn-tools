FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include

OBJETS = filetyp.o

default: absolu

.ftn.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	r.compile -arch $(EC_ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

absolu: $(OBJETS)
	r.build -o r.filetype -obj $(OBJETS) -arch $(EC_ARCH) -abi $(ABI) -librmn rmn_x

clean:
	rm r.filetype $(OBJETS)

