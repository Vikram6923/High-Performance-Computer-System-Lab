CXX := g++
FLAGS := -fopenmp
TARGETS := prefixsum test cas

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	
prefixsum: prefixsum.cpp
	$(CXX) $(FLAGS) -o $@ $<

test: test.cpp
	$(CXX) $(FLAGS) -o $@ $<

cas: cas.cpp
	$(CXX) $(FLAGS) -o $@ $<
