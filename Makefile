# Les options pour le cours INF1
CXXFLAGS= -std=c++11 -Wall -Wconversion -Wextra -pedantic

all2: main2
	./main2
all: main
	./main

main: pieces.o
main2: pieces.o

pieces.o: pieces.cpp pieces.h
