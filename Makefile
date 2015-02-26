.SUFFIXES :

.SUFFIXES : .ftn90 .f90 .ftn .c .o

SHELL = /bin/sh

CPP = /lib/cpp

FFLAGS =

CFLAGS =

OPTIMIZ = -O 2
$(info OPTIMIZ is ${OPTIMIZ})

CPPFLAGS = 

VER = 1.2

LIBRMN = 

default: absolu
#default:
#	@echo dummy target default

.f90.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<

.c.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(CFLAGS)" -src $<

.f.o:
	s.compile -abi $(ABI) $(OPTIMIZ) -opt "=$(FFLAGS)" -src $<


OBJET= r.ip123.o


#absolu: $(OBJET)
absolu:
	rm -f a.out *.o *.mod
	s.compile -src r.ip123.f90 -o r.ip123_$(VER)-$(BASE_ARCH)  -librmn $(LIBRMN)
test:
	rm -f a.out *.o *.mod
#	s.cc -c -I. *.c 
#	s.f77 -c convert_ip.f
#	s.f90 -o a.out test_f_convip.f90  test_c_convip.o convert_ip123.f90
	s.compile -src test_c_convip.c convert_ip123.f90  test_f_convip.f90 -o a.out
	./a.out
#	rm -f a.out *.o *.mod

clean:
	rm -f *.o *.out *.mod *_$(VER)-$(BASE_ARCH)
