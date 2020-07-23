CC=g++
CFLAGS=-w -std=c++11 -O3 
#-w suppresses warning
#enable -DDEBUGMODE for debugging

all: make_directories essC essD

essC:
	$(CC) $(CFLAGS) -o ../aux/essAuxCompress ess.cpp

essD:
	$(CC) $(CFLAGS) -o ../aux/essAuxDecompress decoder.cpp

.PHONY: make_directories
make_directories:
	mkdir -p ../aux/

clean:
	rm -rf *.o ../aux/essAuxCompress ../aux/essAuxDecompress ../aux/essAuxBcalm ../aux/essAuxDsk ../aux/essAuxDsk2ascii ../aux/essAuxMFCompressC ../aux/essAuxMFCompressD ../dependencies/bcalm ../dependencies/dsk 
	cd ../dependencies/mfc1.01; make -f Makefile.linux clean
