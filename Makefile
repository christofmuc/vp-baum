#
#
#

include ../Defines

#
# Der Name des C++ Compilers und Compiler Optionen
#

CC    = gcc

# fuer Linux einkommentieren
FLAGS  = -m486 -ggdb 

# fuer ULTRIX einkommentieren
#FLAGS = -O2 -D_ULTRIX 

PATHS = -DDESTDIR=\"$(DESTDIR)\" -DLIBDIR=\"$(LIBDIR)\"

CCOPT = -Wall $(FLAGS) $(PATHS) -c

#
# Namen der Executables
# 
EXEC  = VPgenerate VPsearch VPksearch VPinfo testset serie positionstest

#
# Die Make Regeln
#

all:    $(EXEC) 

VPgenerate: VPgenerate.o VPBaum.o Seite.o Merkmal.o MerkmalsMenge.o Mass.o KArray.o
	$(CC) -o VPgenerate VPgenerate.o VPBaum.o Seite.o Merkmal.o \
		 MerkmalsMenge.o Mass.o KArray.o -lm

VPsearch: VPsearch.o VPBaum.o Seite.o Merkmal.o MerkmalsMenge.o Mass.o KArray.o
	$(CC) -o VPsearch VPsearch.o VPBaum.o Seite.o Merkmal.o \
		 MerkmalsMenge.o Mass.o KArray.o -lm

VPksearch: VPksearch.o VPBaum.o Seite.o Merkmal.o MerkmalsMenge.o \
       	Mass.o KArray.o
	$(CC) -o VPksearch VPksearch.o VPBaum.o Seite.o Merkmal.o \
		 MerkmalsMenge.o Mass.o KArray.o -lm

VPinfo: VPinfo.o VPBaum.o Seite.o Merkmal.o MerkmalsMenge.o Mass.o KArray.o
	$(CC) -o VPinfo VPinfo.o VPBaum.o Seite.o Merkmal.o \
		 MerkmalsMenge.o Mass.o KArray.o -lm

testset: testset.o MerkmalsMenge.o Merkmal.o
	$(CC) -o testset testset.o MerkmalsMenge.o Merkmal.o

serie: serie.o
	$(CC) -Wall -o serie serie.o

positionstest: positionstest.o
	$(CC) -Wall -o positionstest positionstest.o

dep:	
	rm -f .depend
	make .depend

.depend:
	echo '# program dependencies' > .depend
	$(CC) -MM *.cc *.c >> .depend

.c.o:
	$(CC) $(CCOPT) $<

.cc.o:
	$(CC) $(CCOPT) $<

clean:	
	rm -f *~
	rm -f *.o
	rm -f core

realclean: clean
	rm -f .depend
	rm -f $(AUTO)
	rm -f $(EXEC)

install: all
	cp $(EXEC) $(DIP_PATH_BIN)

backup:
	tar czvf source.tgz *.cc *.c *.hh Makefile
	mcopy -n source.tgz a:
	rm source.tgz 

tarbackup:
	tar czvf /dev/fd0 *.hh *.cc *.c Makefile 

include .depend









