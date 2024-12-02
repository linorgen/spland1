# # Please implement your Makefile rules and targets below.
# # Customize this file to define how to build your project.

# all: clean compile link cleanObjAndrun

# compile:
# 	@echo "compiling source code"
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/main.cpp
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Settlement.cpp
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Facility.cpp
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/selectionpolicy.cpp
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Plan.cpp
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Simulation.cpp
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Action.cpp
# 	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Auxiliary.cpp

# link: bin/main.o bin/Settlement.o bin/Facility.o bin/selectionpolicy.o bin/Plan bin/Simulation.o bin/Action.o bin/Auxiliary.o
# 	@echo "linking object files"
# 	g++ -g -Wall -Weffc++ -std=c++11 -o bin/program bin/main.o bin/Settlement.o bin/Facility.o bin/selectionpolicy.o bin/Plan bin/Simulation.o bin/Action.o bin/Auxiliary.o

# clean:
# 	rm -f bin/*

# cleanObjAndrun:
# 	rm -f *.o
# 	valgrind --leak-check=full --show-reachable=yes ./bin/main
# 	bin/program config_file.txt

# // --------------------

make: all

all: clean compile link run

clean:
	rm -f ./bin/*

compile : src/Auxiliary.cpp src/main.cpp src/Simulation.cpp src/Settlement.cpp src/Facility.cpp src/FacilityType.cpp src/selectionpolicy2.cpp src/Action.cpp src/Plan.cpp
	@echo "compiling source code"
	g++ -c -Wall -g -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ -c -Wall -g -Iinclude -o bin/main.o src/main.cpp
	g++ -c -Wall -g -Iinclude -o bin/Simulation.o src/Simulation.cpp
	g++ -c -Wall -g -Iinclude -o bin/Settlement.o src/Settlement.cpp
	g++ -c -Wall -g -Iinclude -o bin/Facility.o src/Facility.cpp
	g++ -c -Wall -g -Iinclude -o bin/FacilityType.o src/FacilityType.cpp
	g++ -c -Wall -g -Iinclude -o bin/selectionpolicy2.o src/selectionpolicy2.cpp
	g++ -c -Wall -g -Iinclude -o bin/Action.o src/Action.cpp
	g++ -c -Wall -g -Iinclude -o bin/Plan.o src/Plan.cpp


link : bin/main.o bin/Settlement.o bin/Facility.o bin/selectionpolicy.o bin/Plan bin/Simulation.o bin/Action.o bin/Auxiliary.o
	@echo "linking object files"
	g++ -o bin/program -o bin/program bin/main.o bin/Settlement.o bin/Facility.o bin/selectionpolicy.o bin/Plan bin/Simulation.o bin/Action.o bin/Auxiliary.o

plan:
	g++ -c -Wall -g -Iinclude -o bin/Plan.o src/Plan.cpp

run : bin/program
	bin/program config_file.txt