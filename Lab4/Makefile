all: prefixsum reducearray

clean:
	rm -f p r
	
prefixsum: prefixsum.cpp
	g++ -o p -fopenmp prefixsum.cpp

reducearray: reducearray.cpp
	g++ -o r -fopenmp reducearray.cpp