CXX := g++
FLAGS := -fopenmp
TARGETS := lock open cas

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	
lock: lock.cpp
	$(CXX) $(FLAGS) -o $@ $<

open: open.cpp
	$(CXX) $(FLAGS) -o $@ $<

cas: cas.cpp
	$(CXX) $(FLAGS) -o $@ $<