CC=g++
CFLAGS=-c -std=c++11 

SOURCES=decoder.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=decoder

all: $(SOURCES) $(EXECUTABLE)
	rm decoder.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

#rule to build sources
.cpp.o: *.hpp
	$(CC) $(CFLAGS) $< -o $@

#tell that any header change should cause a rebuild
#decoder.cpp: decoder.hpp


decoderd: decoder.cpp
	$(CC) $(CFLAGS) decoder.cpp -o decoder.o
	$(CC) decoder.o -o decoder.out

clean:
	rm -f *.o decoder
	rm -f *.ess *.esstip

clear:
	rm -f *.ess *.esstip
	rm -f *.o

.SILENT:run

test1:
	./decoder -i $(PWD)/ust_ess_tip.txt -k 11 -t 0

test2:
	./decoder -i $(PWD)/ust_ess_abs.txt -k 11 -t 1

