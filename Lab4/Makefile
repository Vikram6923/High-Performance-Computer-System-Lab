all: p1 p2 prob1 prob2

clean:
	rm -f p1 p2 prob1 prob2

p1: p1.c
	gcc -o p1 -fopenmp p1.c

p2: p2.c
	gcc -o p2 -fopenmp p2.c

prob1: prob1.cpp
	g++ -o prob1 -fopenmp prob1.cpp
	./prob1 128 8
	
prob2: prob2.cpp
	g++ -o prob2 -fopenmp prob2.cpp
	./prob2 128 8
