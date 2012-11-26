.SUFFIXES:

.SUFFIXES : .c .ftn .f .o .cdk .ftn90

FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include

VER = 2.1

PROGRAM = r.basename 


default: allbin

.ftn90.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.ftn.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)"  -src $<

.f.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

Linux:  clean
	r.remotemake -as_armnlib Linux $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) Linux $(PROGRAM)

SGI:    clean
	r.remotemake -as_armnlib IRIX64 $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) IRIX64 $(PROGRAM)
#	r.remotemake -as_armnlib IRIX $(PROGRAM) RELS=$(RELS)
#	r.distribute -as_armnlib -rels_$(RELS) IRIX $(PROGRAM)

HP:     clean
	r.remotemake -as_armnlib HP-UX $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) HP-UX $(PROGRAM)

SX4:    clean
	r.remotemake -as_armnlib SX4 $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) -newbin SX4 $(PROGRAM)

SX5:    clean
	r.remotemake -as_armnlib SX5 $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) -newbin SX5 $(PROGRAM)

all:	Linux SGI HP SX4 SX5


r.basename: r.basename.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj r.basename.o -abi $(ABI) -conly

r.ls: r.ls.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj r.ls.o -abi $(ABI) -conly

r.isowner: r.isowner.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj r.isowner.o -abi $(ABI) -conly

r.echo: r.echo.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj r.echo.o -abi $(ABI) -conly

r.split: spliter.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj spliter.o -abi $(ABI) -conly

kmwtopcl: kmwtopcl.o newdx.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj kmwtopcl.o newdx.o -abi $(ABI) -conly

kmwtohp: kmwtohp.o newdx.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj kmwtohp.o newdx.o -abi $(ABI) -conly

r.a2ps: a2ps.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj a2ps.o -abi $(ABI) -conly

text2ps: text2ps.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj text2ps.o -abi $(ABI) -conly

outine: outine.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj outine.o -abi $(ABI) -conly

lpage: lpage.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj lpage.o -abi $(ABI) -conly

nl2crlf: nl2crlf.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj nl2crlf.o -abi $(ABI) -conly

traffic: traffic.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj traffic.o -abi $(ABI) -conly

r.crackres: crackres.o
	s.compile -o $@_$(VER)-$(BASE_ARCH) -obj crackres.o -abi $(ABI) -librmn rmn_x

arbre: arbre.o
	s.compile -o arbre_$(VER)-$(BASE_ARCH) -obj arbre.o -abi $(ABI) -conly

grepdep: grepdep.o
	s.compile -o grepdep_$(VER)-$(BASE_ARCH) -obj grepdep.o -abi $(ABI) -conly

r.ucanrm: r.ucanrm.o
	s.compile -o r.ucanrm_$(VER)-$(BASE_ARCH) -obj r.ucanrm.o -abi $(ABI) -conly

allbin: r.basename r.ls r.isowner r.echo r.split kmwtopcl kmwtohp r.a2ps text2ps outine lpage nl2crlf traffic\
        arbre grepdep r.ucanrm

clean:
	-if [ "*.ftn" != "`echo *.ftn`" ] ; \
	then \
	for i in *.ftn ; \
	do \
	fn=`r.basename $$i '.ftn'`; \
	rm -f $$fn.f; \
	done \
	fi
	-rm *_$(VER)-$(BASE_ARCH) *.o

