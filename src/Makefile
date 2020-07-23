CC=g++
CFLAGS=-w -std=c++11 -O3 
#-w suppresses warning
#enable -DDEBUGMODE for debugging

all: make_directories essC essD

essC:
	$(CC) $(CFLAGS) -o ../bin/essAuxCompress ess.cpp

essD:
	$(CC) $(CFLAGS) -o ../bin/essAuxDecompress decoder.cpp

.PHONY: make_directories
make_directories:
	mkdir -p ../bin/

clean:
	rm -rf *.o ../bin/essAuxCompress ../bin/essAuxDecompress ../bin/essAuxBcalm ../bin/essAuxDsk ../bin/essAuxDsk2ascii ../bin/essAuxMFCompressC ../bin/essAuxMFCompressD ../dependencies/bcalm ../dependencies/dsk 
	cd ../dependencies/mfc1.01; make -f Makefile.linux clean
