CXX := g++
FLAGS := -fopenmp
TARGETS := prefixsum 

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	
prefixsum: prefixsum.cpp
	$(CXX) $(FLAGS) -o $@ $<
