CC=g++
CFLAGS=-w -std=c++11 -O3

all: make_directories essCompress essDecompress

essCompress: 
	$(CC) $(CFLAGS) -o bin/essCompress src/ess.cpp

essDecompress: 
	$(CC) $(CFLAGS) -o bin/essDecompress src/decoder.cpp

.PHONY: make_directories
make_directories: 
	mkdir -p bin/

clean:
	rm -f *.o bin/essDecompress bin/essCompress
