all: compile run 
compile: 
	g++ main.cpp -std=c++11 -o main.out 
run: 
	./main.out 