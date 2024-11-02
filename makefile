CC = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3

all: main

main: main.cpp BOOLEAN_OP.cpp BOOLEAN_OP.hpp
	$(CXX) $(CXXFLAGS) -o $@ $^

run: main
	./main


clean:
	-rm -f *.o main 
