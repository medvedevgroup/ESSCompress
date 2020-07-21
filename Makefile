CC=g++
CFLAGS=-w -std=c++11 -O3 
#-w suppresses warning
#enable -DDEBUGMODE for debugging

all: make_directories essC essD

essC:
	$(CC) $(CFLAGS) -o bin/essC src/ess.cpp

essD:
	$(CC) $(CFLAGS) -o bin/essD src/decoder.cpp

.PHONY: make_directories
make_directories:
	mkdir -p bin/

clean:
	rm -f *.o bin/essD bin/essC
