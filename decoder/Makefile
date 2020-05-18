CC=g++
CFLAGS=-c -std=c++11 

SOURCES=decoder.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=essDecompress

all: $(SOURCES) $(EXECUTABLE)
	rm decoder.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

#rule to build sources
.cpp.o: *.hpp
	$(CC) $(CFLAGS) $< -o $@

#tell that any header change should cause a rebuild
#decoder.cpp: decoder.hpp

clean:
	rm -f *.o essDecompress

clear:
	rm -f *.spss.fa
	rm -f *.o

.SILENT:run

test1:
	./essDecompress -i $(PWD)/chol.ess -k 11 -t 0

test2:
	./essDecompress -i $(PWD)/chol.esstip -k 11 -t 1

