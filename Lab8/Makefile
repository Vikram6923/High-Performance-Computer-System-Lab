CXX := g++
FLAGS := -fopenmp
TARGETS := prefixsum open 

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	
prefixsum: prefixsum.cpp
	$(CXX) $(FLAGS) -o $@ $<

open: open.cpp
	$(CXX) $(FLAGS) -o $@ $<
