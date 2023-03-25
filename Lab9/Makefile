CXX := g++
FLAGS := -fopenmp
TARGETS := lock

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	
lock: lock.cpp
	$(CXX) $(FLAGS) -o $@ $<
