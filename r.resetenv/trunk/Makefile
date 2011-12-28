SHELL= /bin/sh
PROGRAM=r.resetenv

$(PROGRAM): r.resetenv.c
	r.compile -arch $(EC_ARCH) -conly -librmn -o $@_$(EC_ARCH) -src r.resetenv.c
	rm -f *.o

Linux:
	r.remotemake -as_armnlib Linux $(PROGRAM) RELS=$(RELS) 
	r.distribute -as_armnlib -rels_$(RELS) Linux $(PROGRAM)
SGI:
	r.remotemake -as_armnlib IRIX64 $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) IRIX64 $(PROGRAM)
	r.remotemake -as_armnlib IRIX $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) IRIX $(PROGRAM)
HP:
	r.remotemake -as_armnlib HP-UX $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) HP-UX $(PROGRAM)
SX4:
	r.remotemake -as_armnlib SX4 $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) -newbin SX4 $(PROGRAM)
SX5:
	r.remotemake -as_armnlib SX5 $(PROGRAM) RELS=$(RELS)
	r.distribute -as_armnlib -rels_$(RELS) -newbin SX5 $(PROGRAM)

all:	Linux SGI HP SX4 SX5

tests:
	essai a b c d -k1 aaa -k4 -- x y z

clean:
	rm -f *.o r.resetenv_*
