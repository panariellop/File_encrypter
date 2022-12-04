all: compile run 
compile: 
	g++ main.cpp -o main.out 
run: 
	./main.out 