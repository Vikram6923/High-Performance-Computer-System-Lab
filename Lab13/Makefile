CXX := g++
FLAGS := -fopenmp
TARGETS :=  test prefixsum

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

test: test.cpp
	$(CXX) $(FLAGS) -o $@ $<

prefixsum: prefixsum.cpp
	$(CXX) $(FLAGS) -o $@ $<
