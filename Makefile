.SUFFIXES:

.SUFFIXES : .c .ftn .f .o .cdk

FFLAGS =

CFLAGS =

OPTIMIZ = -O 2

CPPFLAGS = -I$(ARMNLIB)/include

PROGRAM = r.basename


default: $(PROGRAM)

.ftn.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)"  -src $<

.f.o:
	r.compile -arch $(ARCH) -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

Linux:  clean
	r.remotemake -as_armnlib Linux $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) Linux $(PROGRAM)

SGI:    clean
	r.remotemake -as_armnlib IRIX64 $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) IRIX64 $(PROGRAM)
	r.remotemake -as_armnlib IRIX $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) IRIX $(PROGRAM)

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
	r.build -o $@_$(ARCH) -obj r.basename.o -arch $(ARCH) -abi $(ABI) -conly

r.ls: r.ls.o
	r.build -o $@_$(ARCH) -obj r.ls.o -arch $(ARCH) -abi $(ABI) -conly

r.isowner: r.isowner.o
	r.build -o $@_$(ARCH) -obj r.isowner.o -arch $(ARCH) -abi $(ABI) -conly

r.echo: r.echo.o
	r.build -o $@_$(ARCH) -obj r.echo.o -arch $(ARCH) -abi $(ABI) -conly

r.split: spliter.o
	r.build -o $@_$(ARCH) -obj spliter.o -arch $(ARCH) -abi $(ABI) -conly

kmwtopcl: kmwtopcl.o newdx.o
	r.build -o $@_$(ARCH) -obj kmwtopcl.o newdx.o -arch $(ARCH) -abi $(ABI) -conly

kmwtohp: kmwtohp.o newdx.o
	r.build -o $@_$(ARCH) -obj kmwtohp.o newdx.o -arch $(ARCH) -abi $(ABI) -conly

r.a2ps: a2ps.o
	r.build -o $@_$(ARCH) -obj a2ps.o -arch $(ARCH) -abi $(ABI) -conly

text2ps: text2ps.o
	r.build -o $@_$(ARCH) -obj text2ps.o -arch $(ARCH) -abi $(ABI) -conly

outine: outine.o
	r.build -o $@_$(ARCH) -obj outine.o -arch $(ARCH) -abi $(ABI) -conly

lpage: lpage.o
	r.build -o $@_$(ARCH) -obj lpage.o -arch $(ARCH) -abi $(ABI) -conly

nl2crlf: nl2crlf.o
	r.build -o $@_$(ARCH) -obj nl2crlf.o -arch $(ARCH) -abi $(ABI) -conly

traffic: traffic.o
	r.build -o $@_$(ARCH) -obj traffic.o -arch $(ARCH) -abi $(ABI) -conly

allbin: r.basename r.ls r.isowner r.echo r.split kmwtopcl kmwtohp r.a2ps text2ps outine lpage nl2crlf traffic

clean:
	-if [ "*.ftn" != "`echo *.ftn`" ] ; \
	then \
	for i in *.ftn ; \
	do \
	fn=`r.basename $$i '.ftn'`; \
	rm -f $$fn.f; \
	done \
	fi
	-rm *_$(ARCH) *.o

