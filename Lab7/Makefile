all: prefixsum open

clean:
	rm -f p o
	
prefixsum: prefixsum.cpp
	g++ -o p -fopenmp prefixsum.cpp

open: open.cpp
	g++ -o o -fopenmp open.cpp