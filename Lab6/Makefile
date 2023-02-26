all: prefixsum compactarray

clean:
	rm -f p r
	
prefixsum: prefixsum.cpp
	g++ -o p -fopenmp prefixsum.cpp

compactarray: compactarray.cpp
	g++ -o r -fopenmp compactarray.cpp