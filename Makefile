all: compile link

compile:
	g++ -c main.cpp
	g++ -c antsim.cpp
	g++ -c ant.cpp
	g++ -c jobs.cpp
	g++ -c job_levels_display.cpp

link:
	g++ main.o antsim.o ant.o jobs.o job_levels_display.o -o main -lsfml-graphics -lsfml-window -lsfml-system
