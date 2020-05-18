CC=g++
CFLAGS=-c -std=c++11 -O2

SOURCES=ess.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ess.out
#BCALMFILE=minitip.unitigs.fa
#K=11

#BCALMFILE=/Volumes/exFAT/data2019/phi/11/list_reads.unitigs.fa


#all: $(SOURCES) $(EXECUTABLE) decoderd
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

#rule to build sources
.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@

#tell that any header change should cause a rebuild
ess.cpp: *.hpp


clean:
	rm -f *.o ess.out incount.txt stitchedUnitigs.txt plainOutput.* stitched* plain* *.fa *.txt global_stat sub_stat

.SILENT:run

test:
	./main.out -i  $(BCALMFILE) -k $(K) -f 1 -m 10 -a 1

clear:
	rm -f global_stat
	rm -f *.txt
	rm -f *.fa

ust:
	./main.out -i  $(BCALMFILE) -k $(K) -m 10
	du -k stitchedUnitigs.fa | cut -f1 

one:
	./main.out -i  $(BCALMFILE) -k $(K) -m 16
	du -k tipOutput.txt | cut -f1 

tip:
	./main.out -i  $(BCALMFILE) -k $(K) -m 15
	du -k tipOutput.txt | cut -f1 

run:
	rm -f global_stat
	./main.out -i  $(BCALMFILE) -k $(K) -m 10
	#./main.out -i  $(BCALMFILE) -k $(K) -f 1 -m 15 > myout.txt
	#./decoder.out -i tipOutput.txt -k $(K) > decot.txt
	#./main.out -i  $(BCALMFILE) -k $(K) -f 1 -m 10 -a 1
	

	#/Volumes/exFAT/work/validation.sh $(BCALMFILE) $(K) 
	#./gzipper.sh

	#./main.out -i /Volumes/exFAT/data2019/chol31/list_reads.unitigs.fa -f 1 -m 11 -k 31 > myout.txt
	#./main.out -i /Volumes/exFAT/data2019/chol/31/list_reads.unitigs.fa  -k 31 -f 1 -m 0 > myout0.txt
	#./main.out -i /Volumes/exFAT/data2019/chol/31/list_reads.unitigs.fa  -k 31 -f 1 -m 10 > myout10.txt
	#./main.out -i /Volumes/exFAT/data2019/staph31/list_reads.unitigs.fa  -k 31 -f 1 -m 0 > myout15.txt
	#cat myout.txt 


	#open myout.txt

	#./validation.sh /Volumes/exFAT/data2019/chol/55/list_reads.unitigs.fa 55  
	
	#du -m plainOutput.txt | cut -f1 
	#gzip plainOutput.txt
	#du -m plainOutput.txt.gz | cut -f1 
