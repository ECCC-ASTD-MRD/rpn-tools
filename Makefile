
.DEFAULT:
	co $@

r.date: r.date.c
	rm -f *.o r.date r.date_$(ARCH)
	r.compile -bidon c -main r_date_main -o r.date -src r.date.c -librmn rmnbeta
	mv r.date r.date_$(ARCH)
	rm -f *.o

all: hp sgi alpha sx5 x86

hp:
	r.remotemake HP-UX r.date
	r.distribute -as_armnlib -rels_$(RELS) HP-UX r.date

sgi:
	r.remotemake IRIX64 r.date
	r.distribute -as_armnlib -rels_$(RELS) IRIX64 r.date

alpha:
	r.remotemake Linux_alpha r.date
	r.distribute -as_armnlib -rels_$(RELS) Linux_alpha r.date

x86:
	r.remotemake Linux r.date
	r.distribute -as_armnlib -rels_$(RELS) Linux r.date

sx5:
	r.remotemake SX5 r.date
	r.distribute -as_armnlib -rels_$(RELS) -newbin SX5 r.date

clean:
	rm -f *.o r.date_*
