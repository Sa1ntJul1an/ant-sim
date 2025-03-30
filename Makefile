all: compile link

compile:
	g++ -c main.cpp
	g++ -c antsim.cpp
	g++ -c ant.cpp

link:
	g++ main.o antsim.o ant.o -o main -lsfml-graphics -lsfml-window -lsfml-system
