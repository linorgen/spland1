# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
all: clean compile link cleanObjAndrun

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp
link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/main *.o

clean:
	rm -f bin/*

cleanObjAndrun:
	rm -f *.o
	# valgrind --leak-check=full --show-reachable=yes ./bin/main