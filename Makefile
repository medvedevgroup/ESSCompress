CC=g++
CFLAGS=-c -std=c++11 
#-O2

SOURCES=ess.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=essCompress


all: $(SOURCES) $(EXECUTABLE)
	rm ess.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

#rule to build sources
.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@

#tell that any header change should cause a rebuild
ess.cpp: *.hpp

clear:
	rm -f *.o *.usttemp global_stat *.ess *.esstip

clean:
	rm -f *.o essCompress

.SILENT:run


test1:
	./essCompress -i  examples/cdbg/chol_k11.unitigs.fa -k 11 -t 0

test2:
	./essCompress -i  examples/cdbg/chol_k11.unitigs.fa -k 11 -t 1